/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_client_session.cc
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

#include "rpi_client_session.h"
#include "../logger/rpi_logger.h"
#include "../rpi_server_config.h"

#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <cstring>

namespace rpi {

ClientSession::ClientSession(int client_fd, std::string client_ip, uint16_t client_port)
    : client_fd_(client_fd), client_ip_(std::move(client_ip)), client_port_(client_port) {
}

ClientSession::~ClientSession() {
    stop();
}

void ClientSession::stop() {
    bool expected = true;
    if (is_running_.compare_exchange_strong(expected, false)) {
        if (client_fd_ >= 0) {
            shutdown(client_fd_, SHUT_RDWR);
            close(client_fd_);
            client_fd_ = -1;
        }
    }
}

bool ClientSession::send_response(const std::string& response) {
    if (client_fd_ < 0 || !is_running_.load()) {
        return false;
    }

    std::string payload = response + "\n";
    const char* ptr = payload.data();
    size_t remaining = payload.size();

    while (remaining > 0 && is_running_.load()) {
        ssize_t bytes_written = send(client_fd_, ptr, remaining, MSG_NOSIGNAL);
        if (bytes_written <= 0) {
            LOG_WARN("Failed to write to client ", client_ip_, ":", client_port_);
            return false;
        }
        ptr += bytes_written;
        remaining -= static_cast<size_t>(bytes_written);
    }

    LOG_DEBUG("Sent response to ", client_ip_, ":", client_port_, " -> ", response);
    return true;
}

void ClientSession::handle() {
    LOG_INFO("Client connected from ", client_ip_, ":", client_port_);

    std::vector<char> buffer(DEFAULT_BUFFER_SIZE);
    std::string line_accumulator;

    while (is_running_.load()) {
        ssize_t bytes_read = recv(client_fd_, buffer.data(), buffer.size(), 0);
        if (bytes_read <= 0) {
            if (bytes_read == 0) {
                LOG_INFO("Client disconnected: ", client_ip_, ":", client_port_);
            } else {
                if (is_running_.load()) {
                    LOG_WARN("Socket read error on client ", client_ip_, ":", client_port_);
                }
            }
            break;
        }

        line_accumulator.append(buffer.data(), static_cast<size_t>(bytes_read));

        size_t newline_pos = std::string::npos;
        while ((newline_pos = line_accumulator.find('\n')) != std::string::npos) {
            std::string line = line_accumulator.substr(0, newline_pos);
            line_accumulator.erase(0, newline_pos + 1);

            // Trim potential CR
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            if (line.empty()) {
                continue;
            }

            LOG_DEBUG("Received raw payload from ", client_ip_, ":", client_port_, " -> ", line);
            auto response = rpc_handler_.process_message(line);
            if (response.has_value()) {
                if (!send_response(response.value())) {
                    is_running_.store(false);
                    break;
                }
            }
        }
    }

    stop();
}

}  // namespace rpi
