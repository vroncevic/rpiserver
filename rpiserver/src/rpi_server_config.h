/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_server_config.h
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

#include <cstddef>
#include <cstdint>
#include <string>

/// @brief Server version string definition
#define RPI_SERVER_VERSION "1.0.0-449c42c"

/// @brief Server executable and application name
#define RPI_SERVER_NAME "rpiserver"

namespace rpi {

//////////////////////////////////////////////////////////////////////////////
/// @brief Default host IP address for binding server socket
constexpr const char* DEFAULT_HOST = "127.0.0.1";

//////////////////////////////////////////////////////////////////////////////
/// @brief Default TCP port number for listening connections
constexpr uint16_t DEFAULT_PORT = 8000;

//////////////////////////////////////////////////////////////////////////////
/// @brief Total number of supported RPI telemetry/control channels
constexpr size_t MAX_CHANNELS = 8;

//////////////////////////////////////////////////////////////////////////////
/// @brief Minimum valid 1-based channel identifier
constexpr int32_t MIN_CHANNEL_ID = 1;

//////////////////////////////////////////////////////////////////////////////
/// @brief Maximum valid 1-based channel identifier
constexpr int32_t MAX_CHANNEL_ID = 8;

//////////////////////////////////////////////////////////////////////////////
/// @brief Minimum valid percentage/intensity value for a channel
constexpr int32_t MIN_CHANNEL_VALUE = 0;

//////////////////////////////////////////////////////////////////////////////
/// @brief Maximum valid percentage/intensity value for a channel
constexpr int32_t MAX_CHANNEL_VALUE = 100;

//////////////////////////////////////////////////////////////////////////////
/// @brief Default I/O buffer size in bytes for network socket streams
constexpr size_t DEFAULT_BUFFER_SIZE = 4096;

}  // namespace rpi
