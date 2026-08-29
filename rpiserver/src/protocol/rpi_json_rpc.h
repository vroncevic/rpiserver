/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_json_rpc.h
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
#include <optional>
#include <string>

namespace rpi {

//////////////////////////////////////////////////////////////////////////////
/// @brief Standard JSON-RPC 2.0 error codes
namespace JsonRpcErrorCode {
    /// @brief Invalid JSON was received by the server
    constexpr int32_t PARSE_ERROR = -32700;

    /// @brief The JSON sent is not a valid Request object
    constexpr int32_t INVALID_REQUEST = -32600;

    /// @brief The method does not exist or is not available
    constexpr int32_t METHOD_NOT_FOUND = -32601;

    /// @brief Invalid method parameter(s)
    constexpr int32_t INVALID_PARAMS = -32602;

    /// @brief Internal JSON-RPC server error
    constexpr int32_t INTERNAL_ERROR = -32603;
}

//////////////////////////////////////////////////////////////////////////////
/// @brief JSON-RPC 2.0 message parser, validator, and response dispatcher
class JsonRpcHandler {
public:
    JsonRpcHandler() = default;
    ~JsonRpcHandler() = default;

    //////////////////////////////////////////////////////////////////////////
    /// @brief Parses a raw JSON request line and executes corresponding action
    /// @param raw_message Raw JSON-RPC 2.0 message string
    /// @return Serialized JSON response string if a response is needed, or nullopt
    std::optional<std::string> process_message(const std::string& raw_message);

private:
    //////////////////////////////////////////////////////////////////////////
    /// @brief Formats a standard JSON-RPC 2.0 error object string
    /// @param code JSON-RPC error code
    /// @param message Error description string
    /// @param id_json Request ID serialized string or "null"
    /// @return Formatted error response JSON string
    std::string make_error_response(int32_t code, const std::string& message, const std::string& id_json = "null");
};

}  // namespace rpi
