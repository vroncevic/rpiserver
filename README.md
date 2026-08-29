# POSIX C++ JSON-RPC 2.0 RPI Server

**rpiserver** is a multi-threaded POSIX C++ TCP server providing JSON-RPC 2.0 remote control and status telemetry for Raspberry Pi nodes, designed to communicate with **[rpiclient](https://github.com/vroncevic/rpiclient)**.

Developed in modern **[C++17](https://en.wikipedia.org/wiki/C%2B%2B17)** with native POSIX sockets and POSIX threads.

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Overview & Features](#overview--features)
- [Installation](#installation)
  - [Autogeneration & Configuration](#autogeneration--configuration)
  - [Compile](#compile)
  - [Install](#install)
- [Usage](#usage)
- [Pairing with rpiclient](#pairing-with-rpiclient)
- [JSON-RPC 2.0 Protocol](#json-rpc-20-protocol)
- [Dependencies](#dependencies)
- [Docs](#docs)
- [Contributing](#contributing)
- [Copyright and Licence](#copyright-and-licence)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

[![rpiserver_build_checker](https://github.com/vroncevic/rpiserver/actions/workflows/rpiserver_build_checker.yml/badge.svg)](https://github.com/vroncevic/rpiserver/actions/workflows/rpiserver_build_checker.yml) [![rpiserver_cc_checker](https://github.com/vroncevic/rpiserver/actions/workflows/rpiserver_cc_checker.yml/badge.svg)](https://github.com/vroncevic/rpiserver/actions/workflows/rpiserver_cc_checker.yml) [![Documentation Status](https://readthedocs.org/projects/rpi-server/badge/?version=master)](https://rpi-server.readthedocs.io/?badge=master) [![GitHub issues open](https://img.shields.io/github/issues/vroncevic/rpiserver.svg)](https://github.com/vroncevic/rpiserver/issues) [![GitHub contributors](https://img.shields.io/github/contributors/vroncevic/rpiserver.svg)](https://github.com/vroncevic/rpiserver/graphs/contributors)

---

### Overview & Features

* **High-Performance POSIX C++ Engine**:
  * Written in modern **C++17** utilizing standard POSIX sockets and multi-threading (`pthreads` / `std::thread`).
  * High-concurrency client connection management with per-session workers.
* **JSON-RPC 2.0 Protocol Compliance**:
  * Line-delimited (`\n`) JSON-RPC 2.0 streaming parser with `nlohmann/json`.
  * Native keep-alive / heartbeat (`REQ`) and channel level adjustment (`SET_CHANNEL`).
* **8-Channel State Management**:
  * Thread-safe channel state manager with mutex synchronization for 8 independent channels (values 0–100).
* **Seamless rpiclient Integration**:
  * Fully interoperable with the GTK-based [rpiclient](https://github.com/vroncevic/rpiclient) desktop application.
* **GNU Autotools Build System**:
  * Standard GNU Autotools pipeline (`autoconf`, `automake`, `libtool`).

---

### Installation

![Debian Linux OS](https://raw.githubusercontent.com/vroncevic/rpiserver/master/docs/debtux.png)

To build and install the server:

#### Autogeneration & Configuration

```bash
cd rpiserver
./autogen.sh
```

Alternatively, if `configure` has already been generated:

```bash
./configure
```

#### Compile

```bash
make
```

#### Install

```bash
sudo make install
```

---

### Usage

Run the server specifying host and port:

```bash
./src/rpiserver --host 127.0.0.1 --port 8000 --verbose
```

Options:
* `-h`, `--host <ip>`: Server host IP address to bind to (default: `127.0.0.1`)
* `-p`, `--port <port>`: Port number to listen on (default: `8000`)
* `-v`, `--verbose`: Enable verbose debug logging
* `-V`, `--version`: Display version information and exit
* `-?`, `--help`: Display usage help

---

### Pairing with rpiclient

To control the node from the graphical user interface, start **rpiserver** on the Raspberry Pi node or local machine:

```bash
./src/rpiserver --host 127.0.0.1 --port 8000
```

Then start [rpiclient](https://github.com/vroncevic/rpiclient):

```bash
./rpiclient/src/rpiclient
```

In **rpiclient**, navigate to **Settings ➔ Network Settings**, configure the target host IP and port (e.g. `127.0.0.1:8000`), and click **Connect**. Telemetry and channel controls will synchronize immediately.

---

### JSON-RPC 2.0 Protocol

All communication between the client and server uses line-delimited (`\n`) JSON-RPC 2.0 messages:

* **Channel Level Control (`SET_CHANNEL`)**:
  * **Request (Client $\rightarrow$ Server)**:
    ```json
    {"jsonrpc": "2.0", "method": "rpi_cmd", "params": {"action": "SET_CHANNEL", "channel": 1, "value": 75}, "id": 101}
    ```
  * **Response (Server $\rightarrow$ Client)**:
    ```json
    {"jsonrpc": "2.0", "result": {"status": "ACK", "channel": 1, "value": 75}, "id": 101}
    ```

* **Heartbeat / Keep-Alive (`REQ`)**:
  * **Request (Client $\rightarrow$ Server)**:
    ```json
    {"jsonrpc": "2.0", "method": "rpi_cmd", "params": {"action": "REQ"}, "id": 102}
    ```
  * **Response (Server $\rightarrow$ Client)**:
    ```json
    {"jsonrpc": "2.0", "result": {"status": "ACK"}, "id": 102}
    ```

---

### Dependencies

This project requires the following dependencies and libraries:

* [C++17 Compiler (g++ or clang++)](https://gcc.gnu.org/)
* [nlohmann-json3-dev](https://github.com/nlohmann/json)
* [pthread](https://man7.org/linux/man-pages/man7/pthreads.7.html)
* [autoconf](https://www.gnu.org/software/autoconf/)
* [automake](https://www.gnu.org/software/automake/)

---

### Docs

More documentation and info at:

* [rpiclient GitHub](https://github.com/vroncevic/rpiclient)
* [rpi-client-gtk.readthedocs.io](https://rpi-client-gtk.readthedocs.io/)
* [nlohmann/json documentation](https://json.nlohmann.me/)

---

### Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request.

---

### Copyright and Licence

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
