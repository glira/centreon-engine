/*
** Copyright 2011 Merethis
**
** This file is part of Centreon Engine.
**
** Centreon Engine is free software: you can redistribute it and/or
** modify it under the terms of the GNU General Public License version 2
** as published by the Free Software Foundation.
**
** Centreon Engine is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Centreon Engine. If not, see
** <http://www.gnu.org/licenses/>.
*/

#include <sstream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>

#include "common.hh"
#include "logging.hh"
#include "globals.hh"
#include "utils.hh"

static int   command_file_fd = -1;
static int   command_file_created = FALSE;
static FILE* command_file_fp = NULL;

/* creates external command file as a named pipe (FIFO) and opens it for reading (non-blocked mode) */
int open_command_file(void) {
  struct stat st;
  int result = 0;

  /* if we're not checking external commands, don't do anything */
  if (config.get_check_external_commands() == false)
    return (OK);

  /* the command file was already created */
  if (command_file_created == TRUE)
    return (OK);

  /* reset umask (group needs write permissions) */
  umask(S_IWOTH);

  /* use existing FIFO if possible */
  if (!(stat(config.get_command_file().toStdString().c_str(), &st) != -1
	&& (st.st_mode & S_IFIFO))) {

    /* create the external command file as a named pipe (FIFO) */
    if ((result = mkfifo(config.get_command_file().toStdString().c_str(),
			 S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) != 0) {

      logit(NSLOG_RUNTIME_ERROR, TRUE,
            "Error: Could not create external command file '%s' as named pipe: (%d) -> %s.  If this file already exists and you are sure that another copy of Centreon Engine is not running, you should delete this file.\n",
            config.get_command_file().toStdString().c_str(),
	    errno,
	    strerror(errno));
      return (ERROR);
    }
  }

  /* open the command file for reading (non-blocked) - O_TRUNC flag cannot be used due to errors on some systems */
  /* NOTE: file must be opened read-write for poll() to work */
  if ((command_file_fd = open(config.get_command_file().toStdString().c_str(), O_RDWR | O_NONBLOCK | O_CLOEXEC)) < 0) {
    logit(NSLOG_RUNTIME_ERROR, TRUE,
          "Error: Could not open external command file for reading via open(): (%d) -> %s\n",
          errno,
	  strerror(errno));
    return (ERROR);
  }

  /* re-open the FIFO for use with fgets() */
  if ((command_file_fp = (FILE*)fdopen(command_file_fd, "r")) == NULL) {
    logit(NSLOG_RUNTIME_ERROR, TRUE,
          "Error: Could not open external command file for reading via fdopen(): (%d) -> %s\n",
          errno,
	  strerror(errno));
    return (ERROR);
  }

  /* initialize worker thread */
  if (init_command_file_worker_thread() == ERROR) {
    logit(NSLOG_RUNTIME_ERROR, TRUE,
          "Error: Could not initialize command file worker thread.\n");

    /* close the command file */
    fclose(command_file_fp);

    /* delete the named pipe */
    unlink(config.get_command_file().toStdString().c_str());

    return (ERROR);
  }

  /* set a flag to remember we already created the file */
  command_file_created = TRUE;

  return (OK);
}

/* closes the external command file FIFO and deletes it */
int close_command_file(void) {
  /* if we're not checking external commands, don't do anything */
  if (config.get_check_external_commands() == false)
    return (OK);

  /* the command file wasn't created or was already cleaned up */
  if (command_file_created == FALSE)
    return (OK);

  /* reset our flag */
  command_file_created = FALSE;

  /* close the command file */
  fclose(command_file_fp);

  return (OK);
}

/* initializes command file worker thread */
int init_command_file_worker_thread(void) {
  int result = 0;
  sigset_t newmask;

  /* initialize circular buffer */
  external_command_buffer.head = 0;
  external_command_buffer.tail = 0;
  external_command_buffer.items = 0;
  external_command_buffer.high = 0;
  external_command_buffer.overflow = 0L;
  external_command_buffer.buffer = new void*[config.get_external_command_buffer_slots()];
  if (external_command_buffer.buffer == NULL)
    return (ERROR);

  /* initialize mutex (only on cold startup) */
  if (sigrestart == FALSE)
    pthread_mutex_init(&external_command_buffer.buffer_lock, NULL);

  /* new thread should block all signals */
  sigfillset(&newmask);
  pthread_sigmask(SIG_BLOCK, &newmask, NULL);

  /* create worker thread */
  result = pthread_create(&worker_threads[COMMAND_WORKER_THREAD],
			  NULL,
			  command_file_worker_thread,
			  NULL);

  /* main thread should unblock all signals */
  pthread_sigmask(SIG_UNBLOCK, &newmask, NULL);

  if (result)
    return (ERROR);

  return (OK);
}

/* shutdown command file worker thread */
int shutdown_command_file_worker_thread(void) {
  int result = 0;

  /*
   * calling pthread_cancel(0) will cause segfaults with some
   * thread libraries. It's possible that will happen if the
   * user has a number of config files larger than the max
   * open file descriptor limit (ulimit -n) and some retarded
   * eventbroker module leaks filedescriptors, since we'll then
   * enter the cleanup() routine from main() before we've
   * spawned any threads.
   */
  if (worker_threads[COMMAND_WORKER_THREAD]) {
    /* tell the worker thread to exit */
    result = pthread_cancel(worker_threads[COMMAND_WORKER_THREAD]);

    /* wait for the worker thread to exit */
    if (result == 0)
      result = pthread_join(worker_threads[COMMAND_WORKER_THREAD], NULL);
    /* we're being called from a fork()'ed child process - can't cancel thread, so just cleanup memory */
    else {
      cleanup_command_file_worker_thread(NULL);
    }
  }

  return (OK);
}

/* clean up resources used by command file worker thread */
void cleanup_command_file_worker_thread(void* arg) {
  int x = 0;

  (void)arg;

  /* release memory allocated to circular buffer */
  for (x = external_command_buffer.tail;
       x != external_command_buffer.head;
       x = (x + 1) % config.get_external_command_buffer_slots()) {
    delete[] ((char**)external_command_buffer.buffer)[x];
    ((char**)external_command_buffer.buffer)[x] = NULL;
  }
  delete[] external_command_buffer.buffer;
  external_command_buffer.buffer = NULL;
}

/* worker thread - artificially increases buffer of named pipe */
void* command_file_worker_thread(void* arg) {
  char input_buffer[MAX_EXTERNAL_COMMAND_LENGTH];
  struct pollfd pfd;
  int pollval;
  struct timeval tv;
  int buffer_items = 0;
  int result = 0;

  (void)arg;

  /* specify cleanup routine */
  pthread_cleanup_push(cleanup_command_file_worker_thread, NULL);

  /* set cancellation info */
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

  while (1) {

    /* should we shutdown? */
    pthread_testcancel();

    /* wait for data to arrive */
    /* select seems to not work, so we have to use poll instead */
    /* 10-15-08 EG check into implementing William's patch @ http://blog.netways.de/2008/08/15/nagios-unter-mac-os-x-installieren/ */
    /* 10-15-08 EG poll() seems broken on OSX - see Jonathan's patch a few lines down */
    pfd.fd = command_file_fd;
    pfd.events = POLLIN;
    pollval = poll(&pfd, 1, 500);

    /* loop if no data */
    if (pollval == 0)
      continue;

    /* check for errors */
    if (pollval == -1) {

      switch (errno) {
      case EBADF:
        logit(logging_options, FALSE, "command_file_worker_thread(): poll(): EBADF");
        break;

      case ENOMEM:
        logit(logging_options, FALSE, "command_file_worker_thread(): poll(): ENOMEM");
        break;

      case EFAULT:
        logit(logging_options, FALSE, "command_file_worker_thread(): poll(): EFAULT");
        break;

      case EINTR:
        /* this can happen when running under a debugger like gdb */
        /*
	  write_to_log("command_file_worker_thread(): poll(): EINTR (impossible)",logging_options,NULL);
	*/
        break;

      default:
        logit(logging_options, FALSE, "command_file_worker_thread(): poll(): Unknown errno value.");
        break;
      }

      continue;
    }

    /* should we shutdown? */
    pthread_testcancel();

    /* get number of items in the buffer */
    pthread_mutex_lock(&external_command_buffer.buffer_lock);
    buffer_items = external_command_buffer.items;
    pthread_mutex_unlock(&external_command_buffer.buffer_lock);

#ifdef DEBUG_CFWT
    printf("(CFWT) BUFFER ITEMS: %d/%d\n", buffer_items,
           config.get_external_command_buffer_slots());
#endif

    /* 10-15-08 Fix for OS X by Jonathan Saggau - see http://www.jonathansaggau.com/blog/2008/09/using_shark_and_custom_dtrace.html */
    /* Not sure if this would have negative effects on other OSes... */
    if (buffer_items == 0) {
      /* pause a bit so OS X doesn't go nuts with CPU overload */
      tv.tv_sec = 0;
      tv.tv_usec = 500;
      select(0, NULL, NULL, NULL, &tv);
    }

    /* process all commands in the file (named pipe) if there's some space in the buffer */
    if (buffer_items < config.get_external_command_buffer_slots()) {

      /* clear EOF condition from prior run (FreeBSD fix) */
      /* FIXME: use_poll_on_cmd_pipe: Still needed? */
      clearerr(command_file_fp);

      /* read and process the next command in the file */
      while (fgets(input_buffer, (int)(sizeof(input_buffer) - 1), command_file_fp) != NULL) {

#ifdef DEBUG_CFWT
        printf("(CFWT) READ: %s", input_buffer);
#endif

        /* submit the external command for processing (retry if buffer is full) */
        while ((result = submit_external_command(input_buffer, &buffer_items)) == ERROR
               && buffer_items == config.get_external_command_buffer_slots()) {

          /* wait a bit */
          tv.tv_sec = 0;
          tv.tv_usec = 250000;
          select(0, NULL, NULL, NULL, &tv);

          /* should we shutdown? */
          pthread_testcancel();
        }

#ifdef DEBUG_CFWT
        printf("(CFWT) RES: %d, BUFFER_ITEMS: %d/%d\n",
	       result,
	       buffer_items,
	       external_comand_buffer_slots);
#endif

        /* bail if the circular buffer is full */
        if (buffer_items == config.get_external_command_buffer_slots())
          break;

        /* should we shutdown? */
        pthread_testcancel();
      }
    }
  }

  /* removes cleanup handler - this should never be reached */
  pthread_cleanup_pop(0);

  return (NULL);
}

/* submits an external command for processing */
int submit_external_command(char* cmd, int* buffer_items) {
  int result = OK;

  if (cmd == NULL || external_command_buffer.buffer == NULL) {
    if (buffer_items != NULL)
      *buffer_items = -1;
    return (ERROR);
  }

  /* obtain a lock for writing to the buffer */
  pthread_mutex_lock(&external_command_buffer.buffer_lock);

  if (external_command_buffer.items < config.get_external_command_buffer_slots()) {

    /* save the line in the buffer */
    ((char**)external_command_buffer.buffer)[external_command_buffer.head] = my_strdup(cmd);

    /* increment the head counter and items */
    external_command_buffer.head = (external_command_buffer.head + 1)
      % config.get_external_command_buffer_slots();
    external_command_buffer.items++;
    if (external_command_buffer.items > external_command_buffer.high)
      external_command_buffer.high = external_command_buffer.items;
  }
  /* buffer was full */
  else
    result = ERROR;

  /* return number of items now in buffer */
  if (buffer_items != NULL)
    *buffer_items = external_command_buffer.items;

  /* release lock on buffer */
  pthread_mutex_unlock(&external_command_buffer.buffer_lock);

  return (result);
}

/* submits a raw external command (without timestamp) for processing */
// XXX: submit_raw_external_command not used.
int submit_raw_external_command(char* cmd, time_t* ts, int* buffer_items) {
  char* newcmd = NULL;
  int result = OK;
  time_t timestamp;

  if (cmd == NULL)
    return (ERROR);

  /* get the time */
  if (ts != NULL)
    timestamp = *ts;
  else
    time(&timestamp);

  /* create the command string */
  std::ostringstream oss;
  oss << '[' << timestamp << "] " << cmd;
  newcmd = my_strdup(oss.str().c_str());

  /* submit the command */
  result = submit_external_command(newcmd, buffer_items);

  /* free allocated memory */
  delete[] newcmd;

  return (result);
}