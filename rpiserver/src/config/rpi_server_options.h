/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_server_options.h
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

#include <cstdint>
#include <string>

namespace rpi {

//////////////////////////////////////////////////////////////////////////////
/// @brief Encapsulates parsed command-line configuration options
struct ServerOptions {
    /// @brief Target IP address to bind server socket to
    std::string host{"127.0.0.1"};

    /// @brief Target TCP port number to listen on
    uint16_t port{8000};

    /// @brief Flag indicating whether verbose debug logging is enabled
    bool verbose{false};

    /// @brief Flag indicating whether usage help was requested
    bool show_help{false};

    /// @brief Flag indicating whether version output was requested
    bool show_version{false};
};

//////////////////////////////////////////////////////////////////////////////
/// @brief CLI options parsing and help/version display utilities
class OptionsParser {
public:
    //////////////////////////////////////////////////////////////////////////
    /// @brief Parses command line arguments and populates ServerOptions
    /// @param argc Number of command line arguments
    /// @param argv Array of command line argument strings
    /// @param options Output struct containing parsed configuration
    /// @return true if arguments were parsed successfully, false on error
    static bool parse(int argc, char* argv[], ServerOptions& options);

    //////////////////////////////////////////////////////////////////////////
    /// @brief Prints program usage and CLI options help to stdout
    /// @param prog_name Executable name from argv[0]
    static void print_usage(const char* prog_name);

    //////////////////////////////////////////////////////////////////////////
    /// @brief Prints application version, copyright, and license to stdout
    static void print_version();
};

}  // namespace rpi
