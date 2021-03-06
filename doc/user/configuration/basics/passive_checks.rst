.. _passive_checks:

Passive Checks
**************

Introduction
============

In most cases you'll use Centreon Engine to monitor your hosts and
services using regularly scheduled
:ref:`active checks <active_checks>`. Active checks can be used to
"poll" a device or service for status information every so
often. Centreon Engine also supports a way to monitor hosts and services
passively instead of actively. They key features of passive checks are
as follows:

  * Passive checks are initiated and performed external
    applications/processes
  * Passive check results are submitted to Centreon Engine for
    processing

The major difference between active and passive checks is that active
checks are initiated and performed by Centreon Engine, while passive
checks are performed by external applications.

Uses For Passive Checks
=======================

Passive checks are useful for monitoring services that are:

  * Asynchronous in nature and cannot be monitored effectively by
    polling their status on a regularly scheduled basis
  * Located behind a firewall and cannot be checked actively from the
    monitoring host

Examples of asynchronous services that lend themselves to being
monitored passively include SNMP traps and security alerts. You never
know how many (if any) traps or alerts you'll receive in a given time
frame, so it's not feasible to just monitor their status every few
minutes.

Passive checks are also used when configuring
:ref:`distributed <distributed_monitoring>`
or :ref:`redundant <redundant_and_failover_monitoring>`
monitoring installations.

How Passive Checks Work
=======================

.. image:: /_static/images/passive_checks.png
   :align: center

Here's how passive checks work in more detail...

  * An external application checks the status of a host or service.
  * The external application writes the results of the check to the
    :ref:`external command file <main_cfg_opt_external_command_file>`.
  * The next time Centreon Engine reads the external command file it
    will place the results of all passive checks into a queue for later
    processing. The same queue that is used for storing results from
    active checks is also used to store the results from passive checks.
  * Centreon Engine will periodically execute a :ref:`check result reaper event <main_cfg_opt_check_result_reaper_frequency>`
    and scan the check result queue. Each service check result that is
    found in the queue is processed in the same manner

The processing of active and passive check results is essentially
identical. This allows for seamless integration of status information
from external applications with Centreon Engine.

Submitting Passive Service Check Results
========================================

External applications can submit passive service check results to
Centreon Engine by writing a PROCESS_SERVICE_CHECK_RESULT
:ref:`external command <external_commands>`
to the external command file.

The format of the command is as follows::

  [<timestamp>] PROCESS_SERVICE_CHECK_RESULT;<host_name>;<svc_description>;<return_code>;<plugin_output>

where...

  * timestamp is the time in time_t format (seconds since the UNIX
    epoch) that the service check was perfomed (or submitted). Please
    note the single space after the right bracket.
  * host_name is the short name of the host associated with the service
    in the service definition
  * svc_description is the description of the service as specified in
    the service definition
  * return_code is the return code of the check (0=OK, 1=WARNING,
    2=CRITICAL, 3=UNKNOWN)
  * plugin_output is the text output of the service check (i.e. the
    plugin output)

.. note::
   A service must be defined in Centreon Engine before you can submit
   passive check results for it! Centreon Engine will ignore all check
   results for services that had not been configured before it was last
   (re)started. An example shell script of how to submit passive service
   check results to Centreon Engine can be found in the documentation on
   :ref:`volatile services <volatile_services>`.

Submitting Passive Host Check Results
=====================================

External applications can submit passive host check results to Centreon
Engine by writing a PROCESS_HOST_CHECK_RESULT external command to the
external command file.

The format of the command is as follows::

  [<timestamp>] PROCESS_HOST_CHECK_RESULT;<host_name>;<host_status>;<plugin_output>

where...

  * timestamp is the time in time_t format (seconds since the UNIX
    epoch) that the host check was perfomed (or submitted). Please note
    the single space after the right bracket.
  * host_name is the short name of the host (as defined in the host
    definition)
  * host_status is the status of the host (0=UP, 1=DOWN, 2=UNREACHABLE)
  * plugin_output is the text output of the host check

.. note::
   A host must be defined in Centreon Engine before you can submit
   passive check results for it! Centreon Engine will ignore all check
   results for hosts that had not been configured before it was last
   (re)started.

Submitting Passive Check Results From Remote Hosts
==================================================

.. image:: /_static/images/nsca.png
   :align: center

If an application that resides on the same host as Centreon Engine is
sending passive host or service check results, it can simply write the
results directly to the external command file as outlined
above. However, applications on remote hosts can't do this so easily.

In order to allow remote hosts to send passive check results to the
monitoring host, I've developed the :ref:`NSCA <addons_nsca>`
addon". The NSCA addon consists of a daemon that runs on the Centreon
Engine hosts and a client that is executed from remote hosts. The daemon
will listen for connections from remote clients, perform some basic
validation on the results being submitted, and then write the check
results directly into the external command file (as described
above). More information on the NSCA addon can be found
:ref:`here <addons_nsca>`.
