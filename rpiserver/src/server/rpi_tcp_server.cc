/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_tcp_server.cc
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

#include "rpi_tcp_server.h"
#include "../logger/rpi_logger.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

namespace rpi {

TCPServer::TCPServer(ServerOptions options)
    : options_(std::move(options)), server_fd_(-1), is_running_(false) {
}

TCPServer::~TCPServer() {
    stop();
}

bool TCPServer::start() {
    if (is_running_.load()) {
        return true;
    }

    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        LOG_ERROR("Failed to create server socket: ", std::strerror(errno));
        return false;
    }

    int optval = 1;
    if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        LOG_WARN("Failed to set SO_REUSEADDR on server socket: ", std::strerror(errno));
    }

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(options_.port);

    if (inet_pton(AF_INET, options_.host.c_str(), &serv_addr.sin_addr) <= 0) {
        LOG_ERROR("Invalid host IP address: ", options_.host);
        close(server_fd_);
        server_fd_ = -1;
        return false;
    }

    if (bind(server_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        LOG_ERROR("Failed to bind server socket to ", options_.host, ":", options_.port, " - ", std::strerror(errno));
        close(server_fd_);
        server_fd_ = -1;
        return false;
    }

    if (listen(server_fd_, SOMAXCONN) < 0) {
        LOG_ERROR("Failed to listen on server socket: ", std::strerror(errno));
        close(server_fd_);
        server_fd_ = -1;
        return false;
    }

    is_running_.store(true);
    LOG_INFO("Starting RPIServer on ", options_.host, ":", options_.port, "...");
    LOG_INFO("RPIServer is running. Press Ctrl+C to terminate.");

    accept_loop();
    return true;
}

void TCPServer::accept_loop() {
    while (is_running_.load()) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        std::memset(&client_addr, 0, sizeof(client_addr));

        int client_fd = accept(server_fd_, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            if (!is_running_.load()) {
                break;
            }
            if (errno == EINTR) {
                continue;
            }
            LOG_WARN("Accept failed: ", std::strerror(errno));
            continue;
        }

        char ip_str[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
        uint16_t client_port = ntohs(client_addr.sin_port);

        auto session = std::make_shared<ClientSession>(client_fd, std::string(ip_str), client_port);

        {
            std::lock_guard<std::mutex> lock(sessions_mutex_);
            sessions_.push_back(session);
            worker_threads_.emplace_back([session]() {
                session->handle();
            });
        }
    }
}

void TCPServer::stop() {
    bool expected = true;
    if (is_running_.compare_exchange_strong(expected, false)) {
        LOG_INFO("Shutting down RPIServer...");

        if (server_fd_ >= 0) {
            shutdown(server_fd_, SHUT_RDWR);
            close(server_fd_);
            server_fd_ = -1;
        }

        {
            std::lock_guard<std::mutex> lock(sessions_mutex_);
            for (auto& session : sessions_) {
                if (session) {
                    session->stop();
                }
            }
        }

        for (auto& worker : worker_threads_) {
            if (worker.joinable()) {
                worker.join();
            }
        }

        {
            std::lock_guard<std::mutex> lock(sessions_mutex_);
            sessions_.clear();
            worker_threads_.clear();
        }

        LOG_INFO("RPIServer stopped successfully.");
    }
}

}  // namespace rpi
