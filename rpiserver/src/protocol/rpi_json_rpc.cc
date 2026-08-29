/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_json_rpc.cc
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

#include "rpi_json_rpc.h"
#include "../core/rpi_channel_manager.h"
#include "../logger/rpi_logger.h"

#include <nlohmann/json.hpp>
#include <algorithm>

namespace rpi {

using json = nlohmann::json;

std::string JsonRpcHandler::make_error_response(int32_t code, const std::string& message, const std::string& id_json) {
    json err_obj = {
        {"jsonrpc", "2.0"},
        {"error", {
            {"code", code},
            {"message", message}
        }}
    };

    if (id_json == "null") {
        err_obj["id"] = nullptr;
    } else {
        try {
            err_obj["id"] = json::parse(id_json);
        } catch (...) {
            err_obj["id"] = id_json;
        }
    }

    return err_obj.dump();
}

std::optional<std::string> JsonRpcHandler::process_message(const std::string& raw_message) {
    json payload;
    try {
        payload = json::parse(raw_message);
    } catch (const json::parse_error& exc) {
        LOG_WARN("Failed to parse JSON request: ", exc.what());
        return make_error_response(JsonRpcErrorCode::PARSE_ERROR, "Parse error", "null");
    }

    if (!payload.is_object()) {
        LOG_WARN("Invalid JSON-RPC payload: not an object");
        return make_error_response(JsonRpcErrorCode::INVALID_REQUEST, "Invalid Request", "null");
    }

    json req_id = nullptr;
    std::string id_str = "null";
    if (payload.contains("id")) {
        req_id = payload["id"];
        id_str = req_id.dump();
    }

    if (!payload.contains("jsonrpc") || payload["jsonrpc"] != "2.0" ||
        !payload.contains("method") || !payload["method"].is_string()) {
        LOG_WARN("Invalid JSON-RPC 2.0 structure");
        return make_error_response(JsonRpcErrorCode::INVALID_REQUEST, "Invalid Request", id_str);
    }

    std::string method = payload["method"].get<std::string>();
    if (method != "rpi_cmd") {
        LOG_WARN("Method not found: ", method);
        return make_error_response(JsonRpcErrorCode::METHOD_NOT_FOUND, "Method not found", id_str);
    }

    if (!payload.contains("params") || !payload["params"].is_object()) {
        LOG_WARN("Missing or invalid 'params' object in rpi_cmd");
        return make_error_response(JsonRpcErrorCode::INVALID_PARAMS, "Invalid params: object expected", id_str);
    }

    const auto& params = payload["params"];
    if (!params.contains("action") || !params["action"].is_string()) {
        LOG_WARN("Missing 'action' in params");
        return make_error_response(JsonRpcErrorCode::INVALID_PARAMS, "Invalid params: 'action' string required", id_str);
    }

    std::string action = params["action"].get<std::string>();
    std::transform(action.begin(), action.end(), action.begin(), ::toupper);

    if (action == "REQ") {
        LOG_INFO("Keepalive REQ received from client (id=", id_str, ")");
        json res = {
            {"jsonrpc", "2.0"},
            {"result", {
                {"status", "ACK"}
            }},
            {"id", req_id}
        };
        return res.dump();
    }

    if (action == "SET_CHANNEL") {
        if (!params.contains("channel") || !params["channel"].is_number_integer() ||
            !params.contains("value") || !params["value"].is_number_integer()) {
            LOG_WARN("SET_CHANNEL missing channel or value integer");
            return make_error_response(JsonRpcErrorCode::INVALID_PARAMS,
                                       "Invalid params: 'channel' and 'value' integers required", id_str);
        }

        int32_t channel = params["channel"].get<int32_t>();
        int32_t value = params["value"].get<int32_t>();

        std::string err_msg;
        if (!ChannelManager::instance().set_channel(channel, value, err_msg)) {
            LOG_WARN("SET_CHANNEL error: ", err_msg);
            return make_error_response(JsonRpcErrorCode::INVALID_PARAMS, err_msg, id_str);
        }

        int32_t clamped_val = std::clamp(value, MIN_CHANNEL_VALUE, MAX_CHANNEL_VALUE);
        json res = {
            {"jsonrpc", "2.0"},
            {"result", {
                {"status", "ACK"},
                {"channel", channel},
                {"value", clamped_val}
            }},
            {"id", req_id}
        };
        return res.dump();
    }

    LOG_WARN("Unknown action requested: ", action);
    return make_error_response(JsonRpcErrorCode::INVALID_PARAMS, "Invalid action: " + action, id_str);
}

}  // namespace rpi
