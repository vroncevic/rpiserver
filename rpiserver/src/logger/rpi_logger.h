/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_logger.h
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

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

namespace rpi {

//////////////////////////////////////////////////////////////////////////////
/// @brief Severity log levels supported by the server logger
enum class LogLevel {
    DEBUG_LEVEL = 0,
    INFO_LEVEL = 1,
    WARNING_LEVEL = 2,
    ERROR_LEVEL = 3
};

//////////////////////////////////////////////////////////////////////////////
/// @brief Thread-safe formatted logging utility
class Logger {
public:
    //////////////////////////////////////////////////////////////////////////
    /// @brief Gets the singleton Logger instance
    /// @return Reference to Logger singleton
    static Logger& instance();

    //////////////////////////////////////////////////////////////////////////
    /// @brief Sets current minimum active logging level
    /// @param level Minimum log level threshold
    void set_level(LogLevel level);

    //////////////////////////////////////////////////////////////////////////
    /// @brief Gets current active logging level
    /// @return Current LogLevel enum value
    LogLevel get_level() const;

    //////////////////////////////////////////////////////////////////////////
    /// @brief Checks if debug level logging is enabled
    /// @return true if debug logs are enabled, false otherwise
    bool is_debug_enabled() const;

    //////////////////////////////////////////////////////////////////////////
    /// @brief Outputs a formatted log entry with timestamp and thread id
    /// @param level Severity level of message
    /// @param message Formatted log string
    void log(LogLevel level, const std::string& message);

    //////////////////////////////////////////////////////////////////////////
    /// @brief Variadic helper for debug log output
    /// @tparam Args Variadic template arguments
    /// @param args Arguments to concatenate into message
    template <typename... Args>
    void debug(Args&&... args) {
        if (is_debug_enabled()) {
            std::ostringstream oss;
            (oss << ... << std::forward<Args>(args));
            log(LogLevel::DEBUG_LEVEL, oss.str());
        }
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief Variadic helper for informational log output
    /// @tparam Args Variadic template arguments
    /// @param args Arguments to concatenate into message
    template <typename... Args>
    void info(Args&&... args) {
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(args));
        log(LogLevel::INFO_LEVEL, oss.str());
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief Variadic helper for warning log output
    /// @tparam Args Variadic template arguments
    /// @param args Arguments to concatenate into message
    template <typename... Args>
    void warning(Args&&... args) {
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(args));
        log(LogLevel::WARNING_LEVEL, oss.str());
    }

    //////////////////////////////////////////////////////////////////////////
    /// @brief Variadic helper for error log output
    /// @tparam Args Variadic template arguments
    /// @param args Arguments to concatenate into message
    template <typename... Args>
    void error(Args&&... args) {
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(args));
        log(LogLevel::ERROR_LEVEL, oss.str());
    }

private:
    Logger();
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    LogLevel level_{LogLevel::INFO_LEVEL};
    mutable std::mutex mutex_;
};

/// @brief Global logging convenience macros
#define LOG_DEBUG(...) ::rpi::Logger::instance().debug(__VA_ARGS__)
#define LOG_INFO(...)  ::rpi::Logger::instance().info(__VA_ARGS__)
#define LOG_WARN(...)  ::rpi::Logger::instance().warning(__VA_ARGS__)
#define LOG_ERROR(...) ::rpi::Logger::instance().error(__VA_ARGS__)

}  // namespace rpi
