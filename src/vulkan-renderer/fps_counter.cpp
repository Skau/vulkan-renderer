﻿#include "inexor/vulkan-renderer/fps_counter.hpp"

namespace inexor::vulkan_renderer {

std::optional<std::uint32_t> FPSCounter::update() {
    auto current_time = std::chrono::high_resolution_clock::now();

    auto time_duration = std::chrono::duration<float, std::chrono::seconds::period>(current_time - last_time).count();

    if (time_duration >= fps_update_interval) {
        auto fps_value = static_cast<std::uint32_t>(frames / time_duration);

        last_time = current_time;
        frames = 0;

        return fps_value;
    }

    frames++;

    return std::nullopt;
}

} // namespace inexor::vulkan_renderer
