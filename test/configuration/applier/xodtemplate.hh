/*
** Copyright 2001-2008 Ethan Galstad
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

#ifndef CCE_XODTEMPLATE_HH
#  define CCE_XODTEMPLATE_HH

#  include "com/centreon/engine/common.hh"

// General definitions.
#  define XODTEMPLATE_NULL                  "null"
#  define MAX_XODTEMPLATE_INPUT_BUFFER      1024
#  define XODTEMPLATE_NONE                  0
#  define XODTEMPLATE_TIMEPERIOD            1
#  define XODTEMPLATE_COMMAND               2
#  define XODTEMPLATE_HOST                  3
#  define XODTEMPLATE_SERVICE               5
#  define XODTEMPLATE_SERVICEDEPENDENCY     6
#  define XODTEMPLATE_HOSTDEPENDENCY        7
#  define XODTEMPLATE_CONNECTOR             9

// Skip lists.
#  define X_HOST_SKIPLIST                   0
#  define X_SERVICE_SKIPLIST                1
#  define X_COMMAND_SKIPLIST                2
#  define X_TIMEPERIOD_SKIPLIST             3
#  define X_HOSTDEPENDENCY_SKIPLIST         6
#  define X_SERVICEDEPENDENCY_SKIPLIST      7
#  define X_CONNECTOR_SKIPLIST              8
#  define NUM_XOBJECT_SKIPLISTS             9


/* CUSTOMVARIABLESMEMBER structure */
typedef struct xodtemplate_customvariablesmember_struct{
  char*         variable_name;
  char*         variable_value;
  struct xodtemplate_customvariablesmember_struct* next;
}              xodtemplate_customvariablesmember;


/* DATERANGE structure */
typedef struct xodtemplate_daterange_struct{
  int          type;
  int          syear;          /* start year */
  int          smon;           /* start month */
  int          smday;          /* start day of month (may 3rd, last day in feb) */
  int          swday;          /* start day of week (thursday) */
  int          swday_offset;   /* start weekday offset (3rd thursday, last monday in jan) */
  int          eyear;
  int          emon;
  int          emday;
  int          ewday;
  int          ewday_offset;
  int          skip_interval;
  char*        timeranges;
  struct xodtemplate_daterange_struct* next;
}              xodtemplate_daterange;


/* TIMEPERIOD TEMPLATE STRUCTURE */
typedef struct xodtemplate_timeperiod_struct{
  char*        tmpl;
  char*        name;
  int          _config_file;
  int          _start_line;

  char*        timeperiod_name;
  char*        alias;
  char*        timeranges[7];
  xodtemplate_daterange* exceptions[DATERANGE_TYPES];
  char*        exclusions;

  int          has_been_resolved;
  int          register_object;
  struct xodtemplate_timeperiod_struct* next;
}              xodtemplate_timeperiod;


/* COMMAND TEMPLATE STRUCTURE */
typedef struct xodtemplate_command_struct{
  char*        tmpl;
  char*        name;
  int          _config_file;
  int          _start_line;

  char*        command_name;
  char*        command_line;

  char*        connector_name;

  int          has_been_resolved;
  int          register_object;
  struct xodtemplate_command_struct* next;
}              xodtemplate_command;

/* CONNECTOR TEMPLATE STRUCTURE */
typedef struct xodtemplate_connector_struct{
  char*        tmpl;
  char*        name;
  int          _config_file;
  int          _start_line;

  char*        connector_name;
  char*        connector_line;

  int          register_object;
  struct xodtemplate_connector_struct* next;
}              xodtemplate_connector;


/* HOST TEMPLATE STRUCTURE */
typedef struct xodtemplate_host_struct{
  char*        tmpl;
  char*        name;
  int          _config_file;
  int          _start_line;

  unsigned int host_id;
  char*        host_name;
  char*        alias;
  char*        address;
  char*        parents;
  char*        check_command;
  char*        check_period;
  unsigned int check_timeout;
  int          initial_state;
  double       check_interval;
  double       retry_interval;
  int          max_check_attempts;
  int          active_checks_enabled;
  int          obsess_over_host;
  char*        event_handler;
  int          event_handler_enabled;
  int          check_freshness;
  int          freshness_threshold;
  float        low_flap_threshold;
  float        high_flap_threshold;
  int          flap_detection_enabled;
  int          flap_detection_on_up;
  int          flap_detection_on_down;
  int          flap_detection_on_unreachable;
  int          notify_on_down;
  int          notify_on_unreachable;
  int          notify_on_recovery;
  int          notify_on_flapping;
  int          notifications_enabled;
  char*        notification_period;
  double       notification_interval;
  double       first_notification_delay;
  char*        timezone;
  xodtemplate_customvariablesmember* custom_variables;

  int          have_parents;
  int          have_check_command;
  int          have_check_period;
  int          have_check_timeout;
  int          have_event_handler;
  int          have_notification_period;
  int          have_timezone;

  int          have_initial_state;
  int          have_check_interval;
  int          have_retry_interval;
  int          have_max_check_attempts;
  int          have_active_checks_enabled;
  int          have_obsess_over_host;
  int          have_event_handler_enabled;
  int          have_check_freshness;
  int          have_freshness_threshold;
  int          have_low_flap_threshold;
  int          have_high_flap_threshold;
  int          have_flap_detection_enabled;
  int          have_flap_detection_options;
  int          have_notification_options;
  int          have_notifications_enabled;
  int          have_notification_interval;
  int          have_first_notification_delay;

  int          has_been_resolved;
  int          register_object;
  struct xodtemplate_host_struct* next;
}              xodtemplate_host;


/* SERVICE TEMPLATE STRUCTURE */
typedef struct xodtemplate_service_struct{
  char*        tmpl;
  char*        name;
  int          _config_file;
  int          _start_line;

  unsigned int host_id;
  char*        host_name;
  unsigned int service_id;
  char*        service_description;
  char*        check_command;
  int          initial_state;
  int          max_check_attempts;
  double       check_interval;
  double       retry_interval;
  char*        check_period;
  unsigned int check_timeout;
  int          active_checks_enabled;
  int          is_volatile;
  int          obsess_over_service;
  char*        event_handler;
  int          event_handler_enabled;
  int          check_freshness;
  int          freshness_threshold;
  double       low_flap_threshold;
  double       high_flap_threshold;
  int          flap_detection_enabled;
  int          flap_detection_on_ok;
  int          flap_detection_on_warning;
  int          flap_detection_on_unknown;
  int          flap_detection_on_critical;
  int          notify_on_unknown;
  int          notify_on_warning;
  int          notify_on_critical;
  int          notify_on_recovery;
  int          notify_on_flapping;
  int          notifications_enabled;
  char*        notification_period;
  double       notification_interval;
  double       first_notification_delay;
  char*        timezone;
  xodtemplate_customvariablesmember* custom_variables;

  int          have_host_name;
  int          have_service_description;
  int          have_check_command;
  int          have_important_check_command;
  int          have_check_period;
  int          have_check_timeout;
  int          have_event_handler;
  int          have_notification_period;
  int          have_timezone;

  int          have_initial_state;
  int          have_max_check_attempts;
  int          have_check_interval;
  int          have_retry_interval;
  int          have_active_checks_enabled;
  int          have_is_volatile;
  int          have_obsess_over_service;
  int          have_event_handler_enabled;
  int          have_check_freshness;
  int          have_freshness_threshold;
  int          have_low_flap_threshold;
  int          have_high_flap_threshold;
  int          have_flap_detection_enabled;
  int          have_flap_detection_options;
  int          have_notification_options;
  int          have_notifications_enabled;
  int          have_notification_dependencies;
  int          have_notification_interval;
  int          have_first_notification_delay;

  int          has_been_resolved;
  int          register_object;
  struct xodtemplate_service_struct* next;
}              xodtemplate_service;


/* SERVICEDEPENDENCY TEMPLATE STRUCTURE */
typedef struct xodtemplate_servicedependency_struct{
  char*        tmpl;
  char*        name;
  int          _config_file;
  int          _start_line;

  char*        host_name;
  char*        service_description;
  char*        dependent_host_name;
  char*        dependent_service_description;
  char*        dependency_period;
  int          inherits_parent;
  int          fail_on_ok;
  int          fail_on_unknown;
  int          fail_on_warning;
  int          fail_on_critical;
  int          fail_on_pending;

  int          have_host_name;
  int          have_service_description;
  int          have_dependent_host_name;
  int          have_dependent_service_description;
  int          have_dependency_period;

  int          have_inherits_parent;
  int          have_dependency_options;

  int          has_been_resolved;
  int          register_object;
  struct xodtemplate_servicedependency_struct* next;
}              xodtemplate_servicedependency;


/* HOSTDEPENDENCY TEMPLATE STRUCTURE */
typedef struct xodtemplate_hostdependency_struct{
  char*        tmpl;
  char*        name;
  int          _config_file;
  int          _start_line;

  char*        host_name;
  char*        dependent_host_name;
  char*        dependency_period;
  int          inherits_parent;
  int          fail_on_up;
  int          fail_on_down;
  int          fail_on_unreachable;
  int          fail_on_pending;

  int          have_host_name;
  int          have_dependent_host_name;
  int          have_dependency_period;

  int          have_inherits_parent;
  int          have_dependency_options;

  int          has_been_resolved;
  int          register_object;
  struct xodtemplate_hostdependency_struct* next;
}              xodtemplate_hostdependency;


/* HOST LIST STRUCTURE */
typedef struct xodtemplate_hostlist_struct{
  char*        host_name;
  struct xodtemplate_hostlist_struct* next;
}              xodtemplate_hostlist;


/* SERVICE LIST STRUCTURE */
typedef struct xodtemplate_servicelist_struct{
  char*        host_name;
  char*        service_description;
  struct xodtemplate_servicelist_struct* next;
}              xodtemplate_servicelist;


/* MEMBER LIST STRUCTURE */
typedef struct xodtemplate_memberlist_struct{
  char*        name1;
  char*        name2;
  struct xodtemplate_memberlist_struct* next;
}              xodtemplate_memberlist;


/***** CHAINED HASH DATA STRUCTURES ******/

typedef struct         xodtemplate_service_cursor_struct{
  int                  xodtemplate_service_iterator;
  xodtemplate_service* current_xodtemplate_service;
}                      xodtemplate_service_cursor;


#  ifdef __cplusplus
extern "C" {
#  endif

int read_main_config_file(char const* main_config_file);

/* top-level routine processes all config files */
int xodtemplate_read_config_data(char const*, int);
/* grabs variables from main config file */
int xodtemplate_grab_config_info(char const*);
/* process data in a specific config file */
int xodtemplate_process_config_file(char*, int);
/* process all files in a specific config directory */
int xodtemplate_process_config_dir(char*, int);

xodtemplate_memberlist* xodtemplate_expand_hosts(
                          char*,
                          int,
                          int);
xodtemplate_memberlist* xodtemplate_expand_services(
                          char*,
                          char*,
                          int,
                          int);

int xodtemplate_add_member_to_memberlist(
      xodtemplate_memberlist**,
      char*,
      char*);
int xodtemplate_free_memberlist(xodtemplate_memberlist **);
void xodtemplate_remove_memberlist_item(
       xodtemplate_memberlist*,
       xodtemplate_memberlist **);

void xodtemplate_free_timeperiod(xodtemplate_timeperiod const* tperiod);

int xodtemplate_begin_object_definition(char*, int, int, int);
int xodtemplate_add_object_property(char*, int);
int xodtemplate_end_object_definition(int);

int xodtemplate_parse_timeperiod_directive(
      xodtemplate_timeperiod*,
      char const*,
      char const*);
xodtemplate_daterange* xodtemplate_add_exception_to_timeperiod(
                         xodtemplate_timeperiod*,
                         int,
                         int,
                         int,
                         int,
                         int,
                         int,
                         int,
                         int,
                         int,
                         int,
                         int,
                         int,
                         char*);
int xodtemplate_get_month_from_string(char*, int*);
int xodtemplate_get_weekday_from_string(char*, int*);

xodtemplate_customvariablesmember* xodtemplate_add_custom_variable_to_host(
                                     xodtemplate_host*,
                                     char*,
                                     char*);
xodtemplate_customvariablesmember* xodtemplate_add_custom_variable_to_service(
                                     xodtemplate_service*,
                                     char*,
                                     char*);
xodtemplate_customvariablesmember* xodtemplate_add_custom_variable_to_object(
                                     xodtemplate_customvariablesmember**,
                                     char*,
                                     char*);


int xodtemplate_register_objects();
int xodtemplate_free_memory();

int xodtemplate_duplicate_objects();
int xodtemplate_duplicate_services();

int xodtemplate_inherit_object_properties();

int xodtemplate_resolve_objects();

int xodtemplate_sort_objects();
int xodtemplate_compare_strings1(char*, char*);
int xodtemplate_compare_strings2(char*, char*, char*, char*);

int xodtemplate_duplicate_service(xodtemplate_service*,char*);
int xodtemplate_duplicate_hostdependency(
      xodtemplate_hostdependency*,
      char*,
      char*);
int xodtemplate_duplicate_servicedependency(
      xodtemplate_servicedependency*,
      char*,
      char*,
      char*,
      char*);

int xodtemplate_resolve_timeperiod(xodtemplate_timeperiod*);
int xodtemplate_resolve_command(xodtemplate_command*);
int xodtemplate_resolve_servicedependency(xodtemplate_servicedependency*);
int xodtemplate_resolve_host(xodtemplate_host*);
int xodtemplate_resolve_service(xodtemplate_service*);
int xodtemplate_resolve_hostdependency(xodtemplate_hostdependency*);

int xodtemplate_sort_timeperiods();
int xodtemplate_sort_commands();
int xodtemplate_sort_connectors();
int xodtemplate_sort_hosts();
int xodtemplate_sort_services();
int xodtemplate_sort_servicedependencies();
int xodtemplate_sort_hostdependencies();

xodtemplate_timeperiod* xodtemplate_find_timeperiod(char*);
xodtemplate_command* xodtemplate_find_command(char*);
xodtemplate_connector* xodtemplate_find_connector(char*);
xodtemplate_servicedependency* xodtemplate_find_servicedependency(char*);
xodtemplate_host* xodtemplate_find_host(char*);
xodtemplate_host* xodtemplate_find_real_host(char*);
xodtemplate_service* xodtemplate_find_service(char*);
xodtemplate_service* xodtemplate_find_real_service(char*,char*);
xodtemplate_hostdependency* xodtemplate_find_hostdependency(char*);

int xodtemplate_get_inherited_string(int*, char**, int*, char **);
int xodtemplate_clean_additive_string(char **);

int xodtemplate_clean_additive_strings();

int xodtemplate_fill_timeperiod(xodtemplate_timeperiod*, timeperiod*);
int xodtemplate_register_timeperiod(xodtemplate_timeperiod*);
int xodtemplate_get_time_ranges(char*, unsigned long*, unsigned long*);

int xodtemplate_register_command(xodtemplate_command*);
int xodtemplate_register_connector(xodtemplate_connector *);
int xodtemplate_register_servicedependency(xodtemplate_servicedependency*);
int xodtemplate_register_host(xodtemplate_host*);
int xodtemplate_register_service(xodtemplate_service*);
int xodtemplate_register_hostdependency(xodtemplate_hostdependency*);


int xodtemplate_init_xobject_skiplists();
int xodtemplate_free_xobject_skiplists();

int xodtemplate_skiplist_compare_text(
      char const* val1a,
      char const* val1b,
      char const* val2a,
      char const* val2b);
int xodtemplate_skiplist_compare_host_template(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_service_template(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_connector_template(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_command_template(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_timeperiod_template(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_hostdependency_template(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_servicedependency_template(
      void const* a,
      void const* b);

int xodtemplate_skiplist_compare_host(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_service(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_command(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_connector(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_timeperiod(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_hostdependency(
      void const* a,
      void const* b);
int xodtemplate_skiplist_compare_servicedependency(
      void const* a,
      void const* b);

#  ifdef __cplusplus
}
#  endif

#endif // !CCE_XODTEMPLATE_HH
