/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_logger.cc
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

#include "rpi_logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>

namespace rpi {

Logger::Logger() : level_(LogLevel::INFO_LEVEL) {}

Logger& Logger::instance() {
    static Logger logger;
    return logger;
}

void Logger::set_level(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    level_ = level;
}

LogLevel Logger::get_level() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return level_;
}

bool Logger::is_debug_enabled() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return level_ <= LogLevel::DEBUG_LEVEL;
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (level < level_) {
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
    localtime_r(&in_time_t, &tm_buf);

    const char* level_str = "INFO";
    switch (level) {
        case LogLevel::DEBUG_LEVEL:
            level_str = "DEBUG";
            break;
        case LogLevel::INFO_LEVEL:
            level_str = "INFO";
            break;
        case LogLevel::WARNING_LEVEL:
            level_str = "WARNING";
            break;
        case LogLevel::ERROR_LEVEL:
            level_str = "ERROR";
            break;
    }

    std::ostream& out = (level >= LogLevel::WARNING_LEVEL) ? std::cerr : std::cout;
    out << "[" << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << level_str << "] "
        << "(thread " << std::this_thread::get_id() << ") "
        << message << std::endl;
}

}  // namespace rpi
