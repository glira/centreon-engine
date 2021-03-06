##
## Copyright 2013-2015 Merethis
##
## This file is part of Centreon Engine.
##
## Centreon Engine is free software: you can redistribute it and/or
## modify it under the terms of the GNU General Public License version 2
## as published by the Free Software Foundation.
##
## Centreon Engine is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Centreon Engine. If not, see
## <http://www.gnu.org/licenses/>.
##

#
# Error tests.
#

# Integer insertion.
set(TEST_NAME "error_insert_integer")
add_executable("${TEST_NAME}" "${TEST_DIR}/error/insert_integer.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# Unsigned integer insertion.
set(TEST_NAME "error_insert_unsigned_integer")
add_executable(
  "${TEST_NAME}"
  "${TEST_DIR}/error/insert_unsigned_integer.cc"
)
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# Long insertion.
set(TEST_NAME "error_insert_long")
add_executable("${TEST_NAME}" "${TEST_DIR}/error/insert_long.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# Long long insertion.
set(TEST_NAME "error_insert_long_long")
add_executable("${TEST_NAME}" "${TEST_DIR}/error/insert_long_long.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# Unsigned long long insertion.
set(TEST_NAME "error_insert_unsigned_long_long")
add_executable(
  "${TEST_NAME}"
  "${TEST_DIR}/error/insert_unsigned_long_long.cc"
)
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# Char insertion.
set(TEST_NAME "error_insert_char")
add_executable("${TEST_NAME}" "${TEST_DIR}/error/insert_char.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# Char array (C string) insertion.
set(TEST_NAME "error_insert_char_array")
add_executable("${TEST_NAME}" "${TEST_DIR}/error/insert_char_array.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# String insertion.
set(TEST_NAME "error_insert_string")
add_executable("${TEST_NAME}" "${TEST_DIR}/error/insert_string.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# Mixed insertions.
set(TEST_NAME "error_insert_mixed")
add_executable("${TEST_NAME}" "${TEST_DIR}/error/insert_mixed.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# Copy constructor.
set(TEST_NAME "error_copy_ctor")
add_executable("${TEST_NAME}" "${TEST_DIR}/error/copy_ctor.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

# Assignment operator.
set(TEST_NAME "error_assignment")
add_executable("${TEST_NAME}" "${TEST_DIR}/error/assignment.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(NAME "${TEST_NAME}" COMMAND "${TEST_NAME}")

#
# Stats tests.
#

# Non-existent configuration file.
set(TEST_NAME "running_stats_no_config_file")
add_test(
  NAME "${TEST_NAME}"
  COMMAND "centenginestats" "-c" "${TEST_DIR}/running/etc/does_not_exist.cfg"
)
set_tests_properties("${TEST_NAME}" PROPERTIES WILL_FAIL TRUE)

# Non-existent status file.
set(TEST_NAME "running_stats_no_status_file")
add_test(
  NAME "${TEST_NAME}"
  COMMAND "centenginestats" "-c" "${TEST_DIR}/running/etc/no_status_file.cfg"
)
set_tests_properties("${TEST_NAME}" PROPERTIES WILL_FAIL TRUE)

#
# Engine tests.
#

set(CENTENGINE_RUNNER "centengine_must_run")
add_executable(
  "${CENTENGINE_RUNNER}"
  "${TEST_DIR}/running/centengine_must_run.cc"
)
target_link_libraries("${CENTENGINE_RUNNER}" "cce_core")

# Help flag.
add_test(NAME "running_engine_help" COMMAND "centengine" "-h")

# License flag.
add_test(NAME "running_engine_license" COMMAND "centengine" "-V")

# Without any service.
add_test(
  NAME "running_engine_no_service"
  COMMAND "centengine" "-v" "${TEST_DIR}/running/etc/no_service.cfg"
)
set_tests_properties(
  "running_engine_no_service"
  PROPERTIES WILL_SUCCESS TRUE
)

# broker_module with only path to module.
set(TEST_NAME "running_engine_broker_module_without_args")
add_test(
  NAME "${TEST_NAME}"
  COMMAND "centengine" "-v" "${TEST_DIR}/running/etc/broker_module_without_args.cfg"
)

# Check that all symbols required by modules are present.
set(TEST_NAME "running_engine_broker_modules_symbols")
add_library(
  "running_engine_broker_modules_symbols_checker"
  SHARED
  "${TEST_DIR}/running/modules_symbols_checker.cc"
)
set_property(
  TARGET "running_engine_broker_modules_symbols_checker"
  PROPERTY PREFIX ""
)
add_library(
  "running_engine_broker_modules_symbols"
  SHARED
  "${TEST_DIR}/running/modules_symbols.cc"
)
set_property(
  TARGET "running_engine_broker_modules_symbols"
  PROPERTY PREFIX ""
)
get_property(
  MODULE_PATH
  TARGET "running_engine_broker_modules_symbols_checker"
  PROPERTY LOCATION
)
get_property(
  MODULE_ARGS
  TARGET "running_engine_broker_modules_symbols"
  PROPERTY LOCATION
)
set(
  CFG_CONTENT
  "command_file=/tmp/running_engine_broker_modules_symbols.tmp
broker_module=${EXTERNALCMD_MODULE}
broker_module=${MODULE_PATH} ${MODULE_ARGS}"
)
configure_file(
  "${TEST_DIR}/running/etc/minimal.cfg.in"
  "${TEST_DIR}/running/etc/modules_symbols.cfg"
)
add_test(
  NAME "${TEST_NAME}"
  COMMAND "centengine" "${TEST_DIR}/running/etc/modules_symbols.cfg"
)
# Check that the webservice module can be loaded.
if (WITH_WEBSERVICE)
  set(TEST_NAME "running_engine_broker_modules_webservice")
  unset(MODULE_ARGS)
  get_property(MODULE_ARGS TARGET "webservice" PROPERTY LOCATION)
  set(CFG_CONTENT
    "command_file=/tmp/running_engine_broker_modules_webservice.tmp
broker_module=${EXTERNALCMD_MODULE}
broker_module=${MODULE_PATH} ${MODULE_ARGS}"
)
  configure_file(
    "${TEST_DIR}/running/etc/minimal.cfg.in"
    "${TEST_DIR}/running/etc/modules_webservice.cfg")
  add_test(
    NAME "${TEST_NAME}"
    COMMAND "centengine" "${TEST_DIR}/running/etc/modules_webservice.cfg"
  )
endif ()

# Active check of host with no command.
set(
  CFG_CONTENT
  "cfg_file=${TEST_DIR}/running/etc/host_with_no_check_cmd_objects.cfg"
)
configure_file(
  "${TEST_DIR}/running/etc/minimal.cfg.in"
  "${TEST_DIR}/running/etc/host_with_no_check_cmd.cfg"
)
set(TEST_NAME "running_host_check_with_no_check_cmd")
add_test(
  NAME "${TEST_NAME}"
  COMMAND "centengine" "${TEST_DIR}/running/etc/host_with_no_check_cmd.cfg"
)
set_tests_properties("${TEST_NAME}" PROPERTIES WILL_FAIL TRUE)

# Active check of service with no command.
set(
  CFG_CONTENT
  "cfg_file=${TEST_DIR}/running/etc/service_with_no_check_cmd_objects.cfg"
)
configure_file(
  "${TEST_DIR}/running/etc/minimal.cfg.in"
  "${TEST_DIR}/running/etc/service_with_no_check_cmd.cfg"
)
set(TEST_NAME "running_service_check_with_no_check_cmd")
add_test(
  NAME "${TEST_NAME}"
  COMMAND "centengine" "-v" "${TEST_DIR}/running/etc/service_with_no_check_cmd.cfg"
)
set_tests_properties("${TEST_NAME}" PROPERTIES WILL_FAIL TRUE)

# Logs are generated for broker modules.
set(TEST_NAME "running_logs_for_modules")
add_library(
  "logs_for_modules"
  SHARED
  "${TEST_DIR}/running/logs_for_modules.cc"
)
unset(MODULE_PATH)
get_property(MODULE_PATH TARGET "logs_for_modules" PROPERTY LOCATION)
set(CFG_CONTENT "broker_module=${MODULE_PATH}")
configure_file(
  "${TEST_DIR}/running/etc/minimal.cfg.in"
  "${TEST_DIR}/running/etc/logs_for_modules.cfg"
)
add_test(
  NAME "${TEST_NAME}"
  COMMAND "centengine" "${TEST_DIR}/running/etc/logs_for_modules.cfg"
)
# log without pid
set(TEST_NAME "running_log_without_pid")
add_executable(
  "${TEST_NAME}"
  "${TEST_DIR}/running/log_without_pid.cc")
target_link_libraries("${TEST_NAME}" "cce_core")
add_test(
  NAME "${TEST_NAME}"
  COMMAND "${TEST_NAME}" "${TEST_DIR}/running/etc/log_without_pid/main.cfg"
)
