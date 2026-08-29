rpiserver
---------

.. image:: https://raw.githubusercontent.com/vroncevic/rpiserver/refs/heads/master/docs/rpi_server_logo.png
   :align: right
   :width: 25%

**rpiserver** is a multi-threaded POSIX C++ TCP server providing JSON-RPC 2.0 remote control and status telemetry for Raspberry Pi nodes, designed to communicate seamlessly with `rpiclient <https://github.com/vroncevic/rpiclient>`_.

Developed in modern **C++17** with native POSIX sockets and POSIX threads.

.. toctree::
   :hidden:

   self

|Build Status| |CC Checker| |Documentation Status| |GitHub issues| |GitHub contributors|

.. |Build Status| image:: https://github.com/vroncevic/rpiserver/actions/workflows/rpiserver_build_checker.yml/badge.svg
   :target: https://github.com/vroncevic/rpiserver/actions/workflows/rpiserver_build_checker.yml

.. |CC Checker| image:: https://github.com/vroncevic/rpiserver/actions/workflows/rpiserver_cc_checker.yml/badge.svg
   :target: https://github.com/vroncevic/rpiserver/actions/workflows/rpiserver_cc_checker.yml

.. |Documentation Status| image:: https://readthedocs.org/projects/rpi-server/badge/?version=master
   :target: https://rpi-server.readthedocs.io/?badge=master

.. |GitHub issues| image:: https://img.shields.io/github/issues/vroncevic/rpiserver.svg
   :target: https://github.com/vroncevic/rpiserver/issues

.. |GitHub contributors| image:: https://img.shields.io/github/contributors/vroncevic/rpiserver.svg
   :target: https://github.com/vroncevic/rpiserver/graphs/contributors

Overview & Features
-------------------

* **High-Performance POSIX C++ Engine**:
  * Written in modern **C++17** utilizing standard POSIX sockets and multi-threading (``pthreads`` / ``std::thread``).
  * High-concurrency client connection management with per-session workers.
* **JSON-RPC 2.0 Protocol Compliance**:
  * Line-delimited (``\n``) JSON-RPC 2.0 streaming parser with `nlohmann/json <https://github.com/nlohmann/json>`_.
  * Native keep-alive / heartbeat (``REQ``) and channel level adjustment (``SET_CHANNEL``).
* **8-Channel State Management**:
  * Thread-safe channel state manager with mutex synchronization for 8 independent channels (values 0–100).
* **Seamless rpiclient Integration**:
  * Fully interoperable with the GTK-based `rpiclient <https://github.com/vroncevic/rpiclient>`_ desktop application.
* **GNU Autotools Build System**:
  * Fully standardized build configuration using ``autoconf``, ``automake``, and ``libtool``.

Installation
------------

.. image:: https://raw.githubusercontent.com/vroncevic/rpiserver/master/docs/debtux.png
   :alt: Debian Linux OS

To build and install the server:

Autogeneration & Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: bash

   cd rpiserver
   ./autogen.sh

Alternatively, if ``configure`` has already been generated:

.. code-block:: bash

   ./configure

Compile
^^^^^^^

.. code-block:: bash

   make

Install
^^^^^^^

.. code-block:: bash

   sudo make install

Usage
-----

Run the server specifying host and port:

.. code-block:: bash

   ./src/rpiserver --host 127.0.0.1 --port 8000 --verbose

Options:

* ``-h``, ``--host <ip>``: Server host IP address to bind to (default: ``127.0.0.1``)
* ``-p``, ``--port <port>``: Port number to listen on (default: ``8000``)
* ``-v``, ``--verbose``: Enable verbose debug logging
* ``-V``, ``--version``: Display version information and exit
* ``-?``, ``--help``: Display usage help

Pairing with rpiclient
----------------------

To control the node from the graphical user interface, start **rpiserver** on the Raspberry Pi node or local machine:

.. code-block:: bash

   ./src/rpiserver --host 127.0.0.1 --port 8000

Then start `rpiclient <https://github.com/vroncevic/rpiclient>`_:

.. code-block:: bash

   ./rpiclient/src/rpiclient

In **rpiclient**, navigate to **Settings ➔ Network Settings**, configure the target host IP and port (e.g. ``127.0.0.1:8000``), and click **Connect**. Telemetry and channel controls will synchronize immediately.

JSON-RPC 2.0 Protocol
---------------------

All communication between the client and server uses line-delimited (``\n``) JSON-RPC 2.0 messages:

* **Channel Level Control (SET_CHANNEL)**:

  Request (Client -> Server):

  .. code-block:: json

     {"jsonrpc": "2.0", "method": "rpi_cmd", "params": {"action": "SET_CHANNEL", "channel": 1, "value": 75}, "id": 101}

  Response (Server -> Client):

  .. code-block:: json

     {"jsonrpc": "2.0", "result": {"status": "ACK", "channel": 1, "value": 75}, "id": 101}

* **Heartbeat / Keep-Alive (REQ)**:

  Request (Client -> Server):

  .. code-block:: json

     {"jsonrpc": "2.0", "method": "rpi_cmd", "params": {"action": "REQ"}, "id": 102}

  Response (Server -> Client):

  .. code-block:: json

     {"jsonrpc": "2.0", "result": {"status": "ACK"}, "id": 102}

Dependencies
------------

This project requires the following dependencies and libraries:

* `C++17 Compiler (g++ or clang++) <https://gcc.gnu.org/>`_
* `nlohmann-json3-dev <https://github.com/nlohmann/json>`_
* `pthread <https://man7.org/linux/man-pages/man7/pthreads.7.html>`_
* `autoconf <https://www.gnu.org/software/autoconf/>`_
* `automake <https://www.gnu.org/software/automake/>`_

Docs
----

More documentation and info at:

* `rpiclient GitHub <https://github.com/vroncevic/rpiclient>`_
* `rpi-client-gtk.readthedocs.io <https://rpi-client-gtk.readthedocs.io/>`_
* `nlohmann/json documentation <https://json.nlohmann.me/>`_

Contributing
------------

1. Fork it!
2. Create your feature branch: ``git checkout -b my-new-feature``
3. Commit your changes: ``git commit -am 'Add some feature'``
4. Push to the branch: ``git push origin my-new-feature``
5. Submit a pull request.

Copyright and Licence
---------------------

.. image:: https://raw.githubusercontent.com/vroncevic/rpiserver/master/docs/foundations.png
   :alt: Foundations

Copyright (c) 2016 - 2026 Vladimir Roncevic <elektron.ronca@gmail.com>

rpiserver is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

rpiserver is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
