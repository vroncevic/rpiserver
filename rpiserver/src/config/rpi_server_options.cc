/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_server_options.cc
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

#include "rpi_server_options.h"
#include "../rpi_server_config.h"

#include <getopt.h>
#include <cstdlib>
#include <iostream>

namespace rpi {

bool OptionsParser::parse(int argc, char* argv[], ServerOptions& options) {
    static struct option long_options[] = {
        {"host", required_argument, nullptr, 'h'},
        {"port", required_argument, nullptr, 'p'},
        {"verbose", no_argument, nullptr, 'v'},
        {"version", no_argument, nullptr, 'V'},
        {"help", no_argument, nullptr, '?'},
        {nullptr, 0, nullptr, 0}
    };

    int opt = 0;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "h:p:vV?", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                if (optarg) {
                    options.host = optarg;
                }
                break;
            case 'p':
                if (optarg) {
                    int p = std::atoi(optarg);
                    if (p > 0 && p <= 65535) {
                        options.port = static_cast<uint16_t>(p);
                    } else {
                        std::cerr << "Error: Invalid port number '" << optarg << "'. Must be in range 1-65535." << std::endl;
                        return false;
                    }
                }
                break;
            case 'v':
                options.verbose = true;
                break;
            case 'V':
                options.show_version = true;
                return true;
            case '?':
                options.show_help = true;
                return true;
            default:
                options.show_help = true;
                return false;
        }
    }

    return true;
}

void OptionsParser::print_usage(const char* prog_name) {
    std::cout << "Usage: " << (prog_name ? prog_name : "rpiserver") << " [OPTIONS]\n\n"
              << "POSIX C++ JSON-RPC 2.0 TCP Server for Raspberry Pi telemetry and control\n\n"
              << "Options:\n"
              << "  -h, --host <ip>     Host IP address to bind to (default: " << DEFAULT_HOST << ")\n"
              << "  -p, --port <port>   Port number to listen on (default: " << DEFAULT_PORT << ")\n"
              << "  -v, --verbose       Enable verbose debug logging\n"
              << "  -V, --version       Display version information and exit\n"
              << "  -?, --help          Display this help message and exit\n"
              << std::endl;
}

void OptionsParser::print_version() {
    std::cout << RPI_SERVER_NAME << " version " << RPI_SERVER_VERSION << "\n"
              << "Copyright (C) 2016 - 2026 Vladimir Roncevic <elektron.ronca@gmail.com>\n"
              << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n"
              << "This is free software: you are free to change and redistribute it.\n"
              << std::endl;
}

}  // namespace rpi
