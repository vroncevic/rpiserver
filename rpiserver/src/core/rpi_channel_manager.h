/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * rpi_channel_manager.h
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

#include "../rpi_server_config.h"

#include <array>
#include <cstdint>
#include <mutex>
#include <string>

namespace rpi {

//////////////////////////////////////////////////////////////////////////////
/// @brief Thread-safe manager for RPI digital/analog channel states
class ChannelManager {
public:
    //////////////////////////////////////////////////////////////////////////
    /// @brief Gets the singleton ChannelManager instance
    /// @return Reference to ChannelManager singleton
    static ChannelManager& instance();

    //////////////////////////////////////////////////////////////////////////
    /// @brief Sets level value for a specific channel
    /// @param channel_id Channel identifier (1-8)
    /// @param value Target percentage value (0-100)
    /// @param error_message Output message populated on failure
    /// @return true if channel was set successfully, false on error
    bool set_channel(int32_t channel_id, int32_t value, std::string& error_message);

    //////////////////////////////////////////////////////////////////////////
    /// @brief Gets current level value of a channel
    /// @param channel_id Channel identifier (1-8)
    /// @param value Output integer to receive channel level
    /// @param error_message Output message populated on failure
    /// @return true if channel was queried successfully, false on error
    bool get_channel(int32_t channel_id, int32_t& value, std::string& error_message) const;

    //////////////////////////////////////////////////////////////////////////
    /// @brief Takes a snapshot of all channel values
    /// @return Array containing all 8 channel levels
    std::array<int32_t, MAX_CHANNELS> get_all_channels() const;

    //////////////////////////////////////////////////////////////////////////
    /// @brief Resets all channel levels to zero
    void reset();

private:
    ChannelManager();
    ~ChannelManager() = default;
    ChannelManager(const ChannelManager&) = delete;
    ChannelManager& operator=(const ChannelManager&) = delete;

    mutable std::mutex mutex_;
    std::array<int32_t, MAX_CHANNELS> channels_{};
};

}  // namespace rpi
