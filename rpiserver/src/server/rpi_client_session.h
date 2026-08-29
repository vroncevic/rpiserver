/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_client_session.h
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

#include "../protocol/rpi_json_rpc.h"

#include <atomic>
#include <cstdint>
#include <string>

namespace rpi {

//////////////////////////////////////////////////////////////////////////////
/// @brief Handles an individual TCP client connection lifecycle and I/O
class ClientSession {
public:
    //////////////////////////////////////////////////////////////////////////
    /// @brief Constructs a new ClientSession instance
    /// @param client_fd Socket file descriptor for client
    /// @param client_ip Remote IP address of client
    /// @param client_port Remote port of client
    ClientSession(int client_fd, std::string client_ip, uint16_t client_port);

    //////////////////////////////////////////////////////////////////////////
    /// @brief Destructor, ensures socket closure
    ~ClientSession();

    ClientSession(const ClientSession&) = delete;
    ClientSession& operator=(const ClientSession&) = delete;

    //////////////////////////////////////////////////////////////////////////
    /// @brief Starts reading and handling requests from the client socket
    void handle();

    //////////////////////////////////////////////////////////////////////////
    /// @brief Stops the session and closes socket descriptor
    void stop();

    //////////////////////////////////////////////////////////////////////////
    /// @brief Gets client socket file descriptor
    /// @return Integer socket descriptor
    int get_fd() const { return client_fd_; }

    //////////////////////////////////////////////////////////////////////////
    /// @brief Gets client remote IP address
    /// @return IP address string
    const std::string& get_ip() const { return client_ip_; }

    //////////////////////////////////////////////////////////////////////////
    /// @brief Gets client remote port
    /// @return Port number
    uint16_t get_port() const { return client_port_; }

private:
    //////////////////////////////////////////////////////////////////////////
    /// @brief Transmits a response string ending with newline over socket
    /// @param response JSON response string
    /// @return true if written successfully, false on socket error
    bool send_response(const std::string& response);

    int client_fd_{-1};
    std::string client_ip_;
    uint16_t client_port_{0};
    std::atomic<bool> is_running_{true};
    JsonRpcHandler rpc_handler_;
};

}  // namespace rpi
