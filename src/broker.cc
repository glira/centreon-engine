/*
** Copyright 2002-2010 Ethan Galstad
** Copyright 2010      Nagios Core Development Team
** Copyright 2011-2015 Merethis
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

#include <unistd.h>
#include "com/centreon/engine/broker.hh"
#include "com/centreon/engine/flapping.hh"
#include "com/centreon/engine/globals.hh"
#include "com/centreon/engine/nebmods.hh"
#include "com/centreon/engine/nebstructs.hh"
#include "com/centreon/engine/sehandlers.hh"
#include "com/centreon/engine/string.hh"

using namespace com::centreon::engine;

extern "C" {

/**
 *  Send adaptive dependency updates to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] data      Target dependency.
 *  @param[in] timestamp Timestamp.
 */
void broker_adaptive_dependency_data(
       int type,
       int flags,
       int attr,
       void* data,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_ADAPTIVE_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_adaptive_dependency_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.object_ptr = data;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_ADAPTIVE_DEPENDENCY_DATA, &ds);
  return;
}

/**
 *  Sends adaptive host updates to broker.
 *
 *  @param[in] type         Type.
 *  @param[in] flags        Flags.
 *  @param[in] attr         Attributes.
 *  @param[in] hst          Target host.
 *  @param[in] command_type Command type.
 *  @param[in] modattr      Global host modified attributes.
 *  @param[in] modattrs     Target host modified attributes.
 *  @param[in] timestamp    Timestamp.
 */
void broker_adaptive_host_data(
       int type,
       int flags,
       int attr,
       host* hst,
       int command_type,
       unsigned long modattr,
       unsigned long modattrs,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_ADAPTIVE_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_adaptive_host_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.command_type = command_type;
  ds.modified_attribute = modattr;
  ds.modified_attributes = modattrs;
  ds.object_ptr = hst;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_ADAPTIVE_HOST_DATA, &ds);
  return;
}

/**
 *  Sends adaptive programs updates to broker.
 *
 *  @param[in] type         Type.
 *  @param[in] flags        Flags.
 *  @param[in] attr         Attributes.
 *  @param[in] command_type Command type.
 *  @param[in] modhattr     Global host modified attributes.
 *  @param[in] modhattrs    Specific host modified attributes.
 *  @param[in] modsattr     Global service modified attributes.
 *  @param[in] modsattrs    Specific service modified attributes.
 *  @param[in] timestamp    Timestamp.
 */
void broker_adaptive_program_data(
       int type,
       int flags,
       int attr,
       int command_type,
       unsigned long modhattr,
       unsigned long modhattrs,
       unsigned long modsattr,
       unsigned long modsattrs,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_ADAPTIVE_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_adaptive_program_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.command_type = command_type;
  ds.modified_host_attribute = modhattr;
  ds.modified_host_attributes = modhattrs;
  ds.modified_service_attribute = modsattr;
  ds.modified_service_attributes = modsattrs;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_ADAPTIVE_PROGRAM_DATA, &ds);
  return;
}

/**
 *  Sends adaptive service updates to broker.
 *
 *  @param[in] type         Type.
 *  @param[in] flags        Flags.
 *  @param[in] attr         Attributes.
 *  @param[in] svc          Target service.
 *  @param[in] command_type Command type.
 *  @param[in] modattr      Global service modified attributes.
 *  @param[in] modattrs     Target service modified attributes.
 *  @param[in] timestamp    Timestamp.
 */
void broker_adaptive_service_data(
       int type,
       int flags,
       int attr,
       service* svc,
       int command_type,
       unsigned long modattr,
       unsigned long modattrs,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_ADAPTIVE_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_adaptive_service_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.command_type = command_type;
  ds.modified_attribute = modattr;
  ds.modified_attributes = modattrs;
  ds.object_ptr = svc;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_ADAPTIVE_SERVICE_DATA, &ds);
  return;
}

/**
 *  Sends adaptive timeperiod updates to broker.
 *
 *  @param[in] type         Type.
 *  @param[in] flags        Flags.
 *  @param[in] attr         Attributes.
 *  @param[in] tp           Target timeperiod.
 *  @param[in] command_type Command type.
 *  @param[in] timestamp    Timestamp.
 */
void broker_adaptive_timeperiod_data(
       int type,
       int flags,
       int attr,
       timeperiod* tp,
       int command_type,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_ADAPTIVE_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_adaptive_timeperiod_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.command_type = command_type;
  ds.object_ptr = tp;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_ADAPTIVE_TIMEPERIOD_DATA, &ds);
  return;
}

/**
 *  Brokers aggregated status dumps.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes
 *  @param[in] timestamp Timestamp.
 */
void broker_aggregated_status_data(
       int type,
       int flags,
       int attr,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_STATUS_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_aggregated_status_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_AGGREGATED_STATUS_DATA, &ds);

  return;
}

/**
 *  Send command data to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] cmd       The command.
 *  @param[in] timestamp Timestamp.
 */
void broker_command_data(
       int type,
       int flags,
       int attr,
       command* cmd,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_COMMAND_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_command_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.cmd = cmd;

  // Make callback.
  neb_make_callbacks(NEBCALLBACK_COMMAND_DATA, &ds);
  return;
}

/**
 *  Sends host custom variables updates to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] data      Host or service.
 *  @param[in] varname   Variable name.
 *  @param[in] varvalue  Variable value.
 *  @param[in] timestamp Timestamp.
 */
void broker_custom_variable(
       int type,
       int flags,
       int attr,
       void* data,
       char const* varname,
       char const* varvalue,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_CUSTOMVARIABLE_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_custom_variable_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.object_ptr = data;
  ds.var_name = const_cast<char*>(varname);
  ds.var_value = const_cast<char*>(varvalue);

  // Make callback.
  neb_make_callbacks(NEBCALLBACK_CUSTOM_VARIABLE_DATA, &ds);

  return;
}

/**
 *  Send event handler data to broker.
 *
 *  @param[in] type              Type.
 *  @param[in] flags             Flags.
 *  @param[in] attr              Attributes.
 *  @param[in] eventhandler_type Event handler type.
 *  @param[in] data              Event handler data.
 *  @param[in] state             State.
 *  @param[in] state_type        State type.
 *  @param[in] start_time        Start time.
 *  @param[in] end_time          End time.
 *  @param[in] exectime          Event handler execution time.
 *  @param[in] timeout           Timeout.
 *  @param[in] early_timeout     Early timeout.
 *  @param[in] retcode           Event handler return code.
 *  @param[in] cmd               Event handler command.
 *  @param[in] cmdline           Command line.
 *  @param[in] output            Output.
 *  @param[in] timestamp         Timestamp.
 *
 *  @return Return value can override event handler execution.
 */
int broker_event_handler(
      int type,
      int flags,
      int attr,
      unsigned int eventhandler_type,
      void* data,
      int state,
      int state_type,
      struct timeval start_time,
      struct timeval end_time,
      double exectime,
      int timeout,
      int early_timeout,
      int retcode,
      char const* cmd,
      char* cmdline,
      char* output,
      struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_EVENT_HANDLERS))
    return (OK);
  if (!data)
    return (ERROR);

  // Get command name/args.
  char* command_buf(NULL);
  char* command_name(NULL);
  char* command_args(NULL);
  if (cmd) {
    command_buf = string::dup(cmd);
    command_name = strtok(command_buf, "!");
    command_args = strtok(NULL, "\x0");
  }

  // Fill struct with relevant data.
  nebstruct_event_handler_data ds;
  host* temp_host(NULL);
  service* temp_service(NULL);
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.eventhandler_type = eventhandler_type;
  if ((eventhandler_type == SERVICE_EVENTHANDLER)
      || (eventhandler_type == GLOBAL_SERVICE_EVENTHANDLER)) {
    temp_service = (service*)data;
    ds.host_name = temp_service->host_name;
    ds.service_description = temp_service->description;
  }
  else {
    temp_host = (host*)data;
    ds.host_name = temp_host->name;
    ds.service_description = NULL;
  }
  ds.object_ptr = data;
  ds.state = state;
  ds.state_type = state_type;
  ds.start_time = start_time;
  ds.end_time = end_time;
  ds.timeout = timeout;
  ds.command_name = command_name;
  ds.command_args = command_args;
  ds.command_line = cmdline;
  ds.early_timeout = early_timeout;
  ds.execution_time = exectime;
  ds.return_code = retcode;
  ds.output = output;

  // Make callbacks.
  int return_code;
  return_code = neb_make_callbacks(NEBCALLBACK_EVENT_HANDLER_DATA, &ds);

  // Free memory.
  delete[] command_buf;
  return (return_code);
}

/**
 *  Sends external commands to broker.
 *
 *  @param[in] type           Type.
 *  @param[in] flags          Flags.
 *  @param[in] attr           Attributes.
 *  @param[in] command_type   Command type.
 *  @param[in] entry_time     Entry time.
 *  @param[in] command_string Command string.
 *  @param[in] command_args   Command args.
 *  @param[in] timestamp      Timestamp.
 */
void broker_external_command(
       int type,
       int flags,
       int attr,
       int command_type,
       time_t entry_time,
       char* command_string,
       char* command_args,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_EXTERNALCOMMAND_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_external_command_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.command_type = command_type;
  ds.entry_time = entry_time;
  ds.command_string = command_string;
  ds.command_args = command_args;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_EXTERNAL_COMMAND_DATA, &ds);

  return;
}

/**
 *  Send flapping data to broker.
 *
 *  @param[in] type           Type.
 *  @param[in] flags          Flags.
 *  @param[in] attr           Attributes.
 *  @param[in] flapping_type  Flapping type.
 *  @param[in] data           Data.
 *  @param[in] percent_change Percent change.
 *  @param[in] high_threshold High threshold.
 *  @param[in] low_threshold  Low threshold.
 *  @param[in] timestamp      Timestamp.
 */
void broker_flapping_data(
       int type,
       int flags,
       int attr,
       unsigned int flapping_type,
       void* data,
       double percent_change,
       double high_threshold,
       double low_threshold,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_FLAPPING_DATA))
    return;
  if (!data)
    return;

  // Fill struct with relevant data.
  nebstruct_flapping_data ds;
  host* temp_host(NULL);
  service* temp_service(NULL);
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.flapping_type = flapping_type;
  if (flapping_type == SERVICE_FLAPPING) {
    temp_service = (service*)data;
    ds.host_name = temp_service->host_name;
    ds.service_description = temp_service->description;
  }
  else {
    temp_host = (host*)data;
    ds.host_name = temp_host->name;
    ds.service_description = NULL;
  }
  ds.object_ptr = data;
  ds.percent_change = percent_change;
  ds.high_threshold = high_threshold;
  ds.low_threshold = low_threshold;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_FLAPPING_DATA, &ds);
  return;
}

/**
 *  Send host check data to broker.
 *
 *  @param[in] type          Type.
 *  @param[in] flags         Flags.
 *  @param[in] attr          Attributes.
 *  @param[in] hst           Host.
 *  @param[in] check_type    Check type.
 *  @param[in] state         State.
 *  @param[in] state_type    State type.
 *  @param[in] start_time    Start time.
 *  @param[in] end_time      End time.
 *  @param[in] cmd           Check command.
 *  @param[in] latency       Latency.
 *  @param[in] exectime      Execution time.
 *  @param[in] timeout       Timeout.
 *  @param[in] early_timeout Early timeout.
 *  @param[in] retcode       Return code.
 *  @param[in] cmdline       Command line.
 *  @param[in] output        Output.
 *  @param[in] long_output   Long output.
 *  @param[in] perfdata      Perfdata.
 *  @param[in] timestamp     Timestamp.
 *
 *  @return Return value can override host check.
 */
int broker_host_check(
      int type,
      int flags,
      int attr,
      host* hst,
      int check_type,
      int state,
      int state_type,
      struct timeval start_time,
      struct timeval end_time,
      char const* cmd,
      double latency,
      double exectime,
      int timeout,
      int early_timeout,
      int retcode,
      char* cmdline,
      char* output,
      char* long_output,
      char* perfdata,
      struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_HOST_CHECKS))
    return (OK);
  if (!hst)
    return (ERROR);

  // Get command name/args.
  char* command_buf(NULL);
  char* command_name(NULL);
  char* command_args(NULL);
  if (cmd) {
    command_buf = string::dup(cmd);
    command_name = strtok(command_buf, "!");
    command_args = strtok(NULL, "\x0");
  }

  // Fill struct with relevant data.
  nebstruct_host_check_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.host_name = hst->name;
  ds.object_ptr = hst;
  ds.check_type = check_type;
  ds.current_attempt = hst->current_attempt;
  ds.max_attempts = hst->max_attempts;
  ds.state = state;
  ds.state_type = state_type;
  ds.timeout = timeout;
  ds.command_name = command_name;
  ds.command_args = command_args;
  ds.command_line = cmdline;
  ds.start_time = start_time;
  ds.end_time = end_time;
  ds.early_timeout = early_timeout;
  ds.execution_time = exectime;
  ds.latency = latency;
  ds.return_code = retcode;
  ds.output = output;
  ds.long_output = long_output;
  ds.perf_data = perfdata;

  // Make callbacks.
  int return_code;
  return_code = neb_make_callbacks(NEBCALLBACK_HOST_CHECK_DATA, &ds);

  // Free data.
  delete[] command_buf;
  return (return_code);
}

/**
 *  Sends host status updates to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] hst       Host.
 *  @param[in] timestamp Timestamp.
 */
void broker_host_status(
       int type,
       int flags,
       int attr,
       host* hst,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_STATUS_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_host_status_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.object_ptr = hst;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_HOST_STATUS_DATA, &ds);
  return;
}

/**
 *  Send log data to broker.
 *
 *  @param[in] type       Type.
 *  @param[in] flags      Flags.
 *  @param[in] attr       Attributes.
 *  @param[in] data       Log entry.
 *  @param[in] data_type  Log type.
 *  @param[in] entry_time Entry time.
 *  @param[in] timestamp  Timestamp.
 */
void broker_log_data(
       int type,
       int flags,
       int attr,
       char* data,
       unsigned long data_type,
       time_t entry_time,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_LOGGED_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_log_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.entry_time = entry_time;
  ds.data_type = data_type;
  ds.data = data;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_LOG_DATA, &ds);
  return;
}

/**
 *  Send module data to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] module    Module.
 *  @param[in] args      Module arguments.
 *  @param[in] timestamp Timestamp.
 */
void broker_module_data(
       int type,
       int flags,
       int attr,
       char const* module,
       char const* args,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_MODULE_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_module_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.module = string::dup(module);
  ds.args = string::dup(args);

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_MODULE_DATA, &ds);

  // Free memory.
  delete[] ds.module;
  delete[] ds.args;
  return;
}

/**
 *  Sends program data (starts, restarts, stops, etc.) to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] timestamp Timestamp.
 */
void broker_program_state(
       int type,
       int flags,
       int attr,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_PROGRAM_STATE))
    return;

  // Fill struct with relevant data.
  nebstruct_process_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_PROCESS_DATA, &ds);
  return;
}

/**
 *  Sends program status updates to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] timestamp Timestamp.
 */
void broker_program_status(
       int type,
       int flags,
       int attr,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_STATUS_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_program_status_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.program_start = program_start;
  ds.pid = getpid();
  ds.daemon_mode=0;
  ds.last_command_check = last_command_check;
  ds.event_handlers_enabled = config->enable_event_handlers();
  ds.flap_detection_enabled = config->enable_flap_detection();
  ds.obsess_over_hosts = config->obsess_over_hosts();
  ds.obsess_over_services = config->obsess_over_services();
  ds.modified_host_attributes = modified_host_process_attributes;
  ds.modified_service_attributes = modified_service_process_attributes;
  ds.global_host_event_handler
    = string::dup(config->global_host_event_handler());
  ds.global_service_event_handler
    = string::dup(config->global_service_event_handler());

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_PROGRAM_STATUS_DATA, &ds);

  // Free memory.
  delete[] ds.global_host_event_handler;
  delete[] ds.global_service_event_handler;
  return;
}

/**
 *  Send relationship data to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] hst       Host.
 *  @param[in] svc       Service (might be null).
 *  @param[in] dep_hst   Dependant host object.
 *  @param[in] dep_svc   Dependant service object (might be null).
 *  @param[in] timestamp Timestamp.
 */
void broker_relation_data(
       int type,
       int flags,
       int attr,
       host* hst,
       service* svc,
       host* dep_hst,
       service* dep_svc,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_RELATION_DATA))
    return;
  if (!hst || !dep_hst)
    return;

  // Fill struct with relevant data.
  nebstruct_relation_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.hst = hst;
  ds.svc = svc;
  ds.dep_hst = dep_hst;
  ds.dep_svc = dep_svc;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_RELATION_DATA, &ds);
  return;
}

/**
 *  Brokers retention data.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] timestamp Timestamp.
 */
void broker_retention_data(
       int type,
       int flags,
       int attr,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_RETENTION_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_retention_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_RETENTION_DATA, &ds);
  return;
}

/**
 *  Send service check data to broker.
 *
 *  @param[in] type          Type.
 *  @param[in] flags         Flags.
 *  @param[in] attr          Attributes.
 *  @param[in] svc           Target service.
 *  @param[in] check_type    Check type.
 *  @param[in] start_time    Start time.
 *  @param[in] end_time      End time.
 *  @param[in] cmd           Service check command.
 *  @param[in] latency       Latency.
 *  @param[in] exectime      Execution time.
 *  @param[in] timeout       Timeout.
 *  @param[in] early_timeout Early timeout.
 *  @param[in] retcode       Return code.
 *  @param[in] cmdline       Check command line.
 *  @param[in] timestamp     Timestamp.
 *
 *  @return Return value can override service check.
 */
int broker_service_check(
      int type,
      int flags,
      int attr,
      service* svc,
      int check_type,
      struct timeval start_time,
      struct timeval end_time,
      char const* cmd,
      double latency,
      double exectime,
      int timeout,
      int early_timeout,
      int retcode,
      char* cmdline,
      struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_SERVICE_CHECKS))
    return (OK);
  if (!svc)
    return (ERROR);

  // Get command name/args.
  char* command_buf(NULL);
  char* command_name(NULL);
  char* command_args(NULL);
  if (cmd) {
    command_buf = string::dup(cmd);
    command_name = strtok(command_buf, "!");
    command_args = strtok(NULL, "\x0");
  }

  // Fill struct with relevant data.
  nebstruct_service_check_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.host_name = svc->host_name;
  ds.service_description = svc->description;
  ds.object_ptr = svc;
  ds.check_type = check_type;
  ds.current_attempt = svc->current_attempt;
  ds.max_attempts = svc->max_attempts;
  ds.state = svc->current_state;
  ds.state_type = svc->state_type;
  ds.timeout = timeout;
  ds.command_name = command_name;
  ds.command_args = command_args;
  ds.command_line = cmdline;
  ds.start_time = start_time;
  ds.end_time = end_time;
  ds.early_timeout = early_timeout;
  ds.execution_time = exectime;
  ds.latency = latency;
  ds.return_code = retcode;
  ds.output = svc->plugin_output;
  ds.long_output = svc->long_plugin_output;
  ds.perf_data = svc->perf_data;

  // Make callbacks.
  int return_code;
  return_code = neb_make_callbacks(
                  NEBCALLBACK_SERVICE_CHECK_DATA,
                  &ds);

  // Free data.
  delete[] command_buf;
  return (return_code);
}

/**
 *  Sends service status updates to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] svc       Target service.
 *  @param[in] timestamp Timestamp.
 */
void broker_service_status(
       int type,
       int flags,
       int attr,
       service* svc,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_STATUS_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_service_status_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.object_ptr = svc;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_SERVICE_STATUS_DATA, &ds);
  return;
}

/**
 *  Send state change data to broker.
 *
 *  @param[in] type             Type.
 *  @param[in] flags            Flags.
 *  @param[in] attr             Attributes.
 *  @param[in] statechange_type State change type.
 *  @param[in] data             Data.
 *  @param[in] state            State.
 *  @param[in] state_type       State type.
 *  @param[in] current_attempt  Current attempt.
 *  @param[in] max_attempts     Max attempts.
 *  @param[in] timestamp        Timestamp.
 */
void broker_statechange_data(
       int type,
       int flags,
       int attr,
       int statechange_type,
       void* data,
       int state,
       int state_type,
       int current_attempt,
       int max_attempts,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_STATECHANGE_DATA))
    return;

  // Fill struct with relevant data.
  nebstruct_statechange_data ds;
  host* temp_host(NULL);
  service* temp_service(NULL);
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.statechange_type = statechange_type;
  if (statechange_type == SERVICE_STATECHANGE) {
    temp_service = (service*)data;
    ds.host_name = temp_service->host_name;
    ds.service_description = temp_service->description;
    ds.output = temp_service->plugin_output;
  }
  else {
    temp_host = (host*)data;
    ds.host_name = temp_host->name;
    ds.service_description = NULL;
    ds.output = temp_host->plugin_output;
  }
  ds.object_ptr = data;
  ds.state = state;
  ds.state_type = state_type;
  ds.current_attempt = current_attempt;
  ds.max_attempts = max_attempts;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_STATE_CHANGE_DATA, &ds);
  return;
}

/**
 *  Send system command data to broker.
 *
 *  @param[in] type          Type.
 *  @param[in] flags         Flags.
 *  @param[in] attr          Attributes.
 *  @param[in] start_time    Start time.
 *  @param[in] end_time      End time.
 *  @param[in] exectime      Execution time.
 *  @param[in] timeout       Timeout.
 *  @param[in] early_timeout Early timeout.
 *  @param[in] retcode       Return code.
 *  @param[in] cmd           Command.
 *  @param[in] output        Output.
 *  @param[in] timestamp     Timestamp.
 */
void broker_system_command(
       int type,
       int flags,
       int attr,
       struct timeval start_time,
       struct timeval end_time,
       double exectime,
       int timeout,
       int early_timeout,
       int retcode,
       char* cmd,
       char* output,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_SYSTEM_COMMANDS))
    return;
  if (!cmd)
    return;

  // Fill struct with relevant data.
  nebstruct_system_command_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.start_time = start_time;
  ds.end_time = end_time;
  ds.timeout = timeout;
  ds.command_line = cmd;
  ds.early_timeout = early_timeout;
  ds.execution_time = exectime;
  ds.return_code = retcode;
  ds.output = output;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_SYSTEM_COMMAND_DATA, &ds);
  return;
}

/**
 *  Send timed event data to broker.
 *
 *  @param[in] type      Type.
 *  @param[in] flags     Flags.
 *  @param[in] attr      Attributes.
 *  @param[in] event     Target event.
 *  @param[in] timestamp Timestamp.
 */
void broker_timed_event(
       int type,
       int flags,
       int attr,
       timed_event* event,
       struct timeval const* timestamp) {
  // Config check.
  if (!(config->event_broker_options() & BROKER_TIMED_EVENTS))
    return;
  if (!event)
    return;

  // Fill struct with relevant data.
  nebstruct_timed_event_data ds;
  ds.type = type;
  ds.flags = flags;
  ds.attr = attr;
  ds.timestamp = get_broker_timestamp(timestamp);
  ds.event_type = event->event_type;
  ds.recurring = event->recurring;
  ds.run_time = event->run_time;
  ds.event_data = event->event_data;
  ds.event_ptr = event;

  // Make callbacks.
  neb_make_callbacks(NEBCALLBACK_TIMED_EVENT_DATA, &ds);
  return;
}

/**
 *  Gets timestamp for use by broker.
 *
 *  @param[in] timestamp Timestamp.
 */
struct timeval get_broker_timestamp(struct timeval const* timestamp) {
  struct timeval tv;
  if (!timestamp)
    gettimeofday(&tv, NULL);
  else
    tv = *timestamp;
  return (tv);
}

}
