/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_channel_manager.cc
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

#include "rpi_channel_manager.h"
#include "../logger/rpi_logger.h"

#include <algorithm>

namespace rpi {

ChannelManager::ChannelManager() {
    channels_.fill(0);
}

ChannelManager& ChannelManager::instance() {
    static ChannelManager manager;
    return manager;
}

bool ChannelManager::set_channel(int32_t channel_id, int32_t value, std::string& error_message) {
    if (channel_id < MIN_CHANNEL_ID || channel_id > MAX_CHANNEL_ID) {
        error_message = "Invalid channel: " + std::to_string(channel_id) + ". Expected range 1-8.";
        return false;
    }

    int32_t clamped_value = std::clamp(value, MIN_CHANNEL_VALUE, MAX_CHANNEL_VALUE);

    {
        std::lock_guard<std::mutex> lock(mutex_);
        channels_[static_cast<size_t>(channel_id - 1)] = clamped_value;
    }

    LOG_INFO("SET_CHANNEL applied: Channel=", channel_id, ", Value=", clamped_value);
    return true;
}

bool ChannelManager::get_channel(int32_t channel_id, int32_t& value, std::string& error_message) const {
    if (channel_id < MIN_CHANNEL_ID || channel_id > MAX_CHANNEL_ID) {
        error_message = "Invalid channel: " + std::to_string(channel_id) + ". Expected range 1-8.";
        return false;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        value = channels_[static_cast<size_t>(channel_id - 1)];
    }

    return true;
}

std::array<int32_t, MAX_CHANNELS> ChannelManager::get_all_channels() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return channels_;
}

void ChannelManager::reset() {
    std::lock_guard<std::mutex> lock(mutex_);
    channels_.fill(0);
    LOG_INFO("All channels reset to 0.");
}

}  // namespace rpi
