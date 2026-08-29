/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_tcp_server.h
 * Copyright (C) 2016 - 2026 Vladimir Roncevic <elektron.ronca@gmail.com>
 *
 * rpiserver is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rpiserver is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "rpi_client_session.h"
#include "../config/rpi_server_options.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace rpi {

//////////////////////////////////////////////////////////////////////////////
/// @brief Multi-threaded POSIX TCP server for incoming JSON-RPC connections
class TCPServer {
public:
    //////////////////////////////////////////////////////////////////////////
    /// @brief Constructs a new TCPServer with specified configuration options
    /// @param options ServerOptions struct with host, port, verbose flags
    explicit TCPServer(ServerOptions options);

    //////////////////////////////////////////////////////////////////////////
    /// @brief Destructor, ensures server stop and thread joining
    ~TCPServer();

    TCPServer(const TCPServer&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;

    //////////////////////////////////////////////////////////////////////////
    /// @brief Binds socket, starts listening, and enters connection accept loop
    /// @return true if server started successfully, false on error
    bool start();

    //////////////////////////////////////////////////////////////////////////
    /// @brief Signals server shutdown and terminates active client connections
    void stop();

    //////////////////////////////////////////////////////////////////////////
    /// @brief Checks if server is actively running
    /// @return true if running, false if stopped
    bool is_running() const { return is_running_.load(); }

private:
    //////////////////////////////////////////////////////////////////////////
    /// @brief Main connection accept loop executed in server thread
    void accept_loop();

    //////////////////////////////////////////////////////////////////////////
    /// @brief Cleans up terminated worker threads and sessions
    void cleanup_threads();

    ServerOptions options_;
    int server_fd_{-1};
    std::atomic<bool> is_running_{false};
    std::thread accept_thread_;

    mutable std::mutex sessions_mutex_;
    std::vector<std::shared_ptr<ClientSession>> sessions_;
    std::vector<std::thread> worker_threads_;
};

}  // namespace rpi
