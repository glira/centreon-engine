.. _passive_host_state_translation:

Passive Host State Translation
******************************

Introduction
============

When Centreon Engine receives passive host checks from remote sources
(i.e other Centreon Engine instances in distributed or failover setups),
the host state reported by the remote source may not accurately reflect
the state of the host from Centreon Engine' view. As distributed and
failover monitoring installations are fairly common, it is important to
provide a mechanism for ensuring accurate host states between different
instances of Centreon Engine.

Different World Views
=====================

The image below shows a simplified view of a failover monitoring setup.

  * Centreon Engine-A is the primary monitoring server, and is actively
    monitoring all switches and routers.
  * Centreon Engine-B and Centreon Engine-C are backup monitoring
    servers, and are receiving passive check results from Centreon
    Engine-A
  * Both Router-C and Router-D have suffered failures and are offline.

.. image:: /_static/images/passivehosttranslation.png
   :align: center

What states are Router-C and Router-D currently in? The answer depends
on which Centreon Engine instance you ask.

  * Centreon Engine-A sees Router-D as DOWN and Router-C as UNREACHABLE
  * Centreon Engine-B should see Router-C as DOWN and Router-D as
    UNREACHABLE
  * Centreon Engine-C should see both routers as being DOWN.

Each Centreon Engine instance has a different view of the network. The
backup monitoring servers should not blindly accept passive host states
from the primary monitoring server, or they will have incorrect
information on the current state of the network.

Without translating passive host check results from the primary
monitoring server (Centreon Engine-A), Centreon Engine-C would see
Router-D as UNREACHABLE, when it is really DOWN based on its
viewpoint. Similarly, the DOWN/UNREACHABLE states (from the viewpoint of
Centreon Engine-A) for Router-C and Router-D should be flipped from the
viewpoint of Centreon Engine-B.
