/*
** Copyright 2011-2012 Merethis
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

#include <exception>
#include <QCoreApplication>
#include <QDebug>
#include "com/centreon/engine/commands/connector/command.hh"
#include "com/centreon/engine/error.hh"
#include "test/unittest.hh"

using namespace com::centreon::engine;
using namespace com::centreon::engine::commands;

#define DEFAULT_CMD_NAME    "cmd"
#define DEFAULT_CMD_LINE    "ls -la /tmp"
#define DEFAULT_CMD_PROCESS "./bin_connector_test_run"

/**
 *  Check getter return.
 */
int main_test() {
  connector::command cmd(DEFAULT_CMD_NAME,
                         DEFAULT_CMD_LINE,
                         DEFAULT_CMD_PROCESS);

  if (cmd.get_name() != DEFAULT_CMD_NAME)
    throw (engine_error() << "error: name invalid value.");

  if (cmd.get_command_line() != DEFAULT_CMD_LINE)
    throw (engine_error() << "error: command_line invalid value.");

  if (cmd.get_process() != DEFAULT_CMD_PROCESS)
    throw (engine_error() << "error: process invalid value.");

  return (0);
}

/**
 *  Init the unit test.
 */
int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);
  unittest utest(&main_test);
  QObject::connect(&utest, SIGNAL(finished()), &app, SLOT(quit()));
  utest.start();
  app.exec();
  return (utest.ret());
}