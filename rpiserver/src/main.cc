/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
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

#include "config/rpi_server_options.h"
#include "logger/rpi_logger.h"
#include "rpi_server_config.h"
#include "server/rpi_tcp_server.h"

#include <csignal>
#include <cstdlib>
#include <iostream>

namespace {
rpi::TCPServer* g_server_ptr = nullptr;

void signal_handler(int signum) {
    if (signum == SIGINT || signum == SIGTERM) {
        if (g_server_ptr != nullptr) {
            g_server_ptr->stop();
        }
    }
}
}  // namespace

int main(int argc, char* argv[]) {
    rpi::ServerOptions options;
    if (!rpi::OptionsParser::parse(argc, argv, options)) {
        rpi::OptionsParser::print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (options.show_version) {
        rpi::OptionsParser::print_version();
        return EXIT_SUCCESS;
    }

    if (options.show_help) {
        rpi::OptionsParser::print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (options.verbose) {
        rpi::Logger::instance().set_level(rpi::LogLevel::DEBUG_LEVEL);
    } else {
        rpi::Logger::instance().set_level(rpi::LogLevel::INFO_LEVEL);
    }

    struct sigaction sa{};
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);

    // Ignore SIGPIPE to avoid abnormal termination on client socket reset
    struct sigaction sa_pipe{};
    sa_pipe.sa_handler = SIG_IGN;
    sigemptyset(&sa_pipe.sa_mask);
    sa_pipe.sa_flags = 0;
    sigaction(SIGPIPE, &sa_pipe, nullptr);

    rpi::TCPServer server(options);
    g_server_ptr = &server;

    if (!server.start()) {
        LOG_ERROR("Failed to start server.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
