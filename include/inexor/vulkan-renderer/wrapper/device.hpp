#pragma once

#include "inexor/vulkan-renderer/availability_checks.hpp"
#include "inexor/vulkan-renderer/settings_decision_maker.hpp"

#include <spdlog/spdlog.h>
#include <vulkan/vulkan.h>

#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace inexor::vulkan_renderer::wrapper {

/// @brief A RAII wrapper for VkDevice, VkPhysicalDevice and VkQueues.
class Device {
private:
    VkDevice device;
    VkPhysicalDevice graphics_card;
    VkQueue graphics_queue;
    VkQueue present_queue;
    VkQueue transfer_queue;
    VkSurfaceKHR surface;

    std::optional<std::uint32_t> present_queue_family_index = std::nullopt;
    std::optional<std::uint32_t> graphics_queue_family_index = std::nullopt;
    std::optional<std::uint32_t> transfer_queue_family_index = std::nullopt;

    AvailabilityChecksManager availability_checks;
    VulkanSettingsDecisionMaker settings_decision_maker;

public:
    /// Delete the copy constructor so shaders are move-only objects.
    Device(const Device &) = delete;
    Device(Device &&other) noexcept;

    /// Delete the copy assignment operator so shaders are move-only objects.
    Device &operator=(const Device &) = delete;
    Device &operator=(Device &&) noexcept = default;

    /// @brief Creates a graphics card interface.
    /// @param preferred_gpu_index [in] The index of the preferred physical device to use.
    Device(const VkInstance instance, const VkSurfaceKHR surface, bool enable_vulkan_debug_markers, bool prefer_distinct_transfer_queue,
           const std::optional<std::uint32_t> preferred_physical_device_index = std::nullopt);

    // TODO: Add overloaded constructors for VkPhysicalDeviceFeatures and requested device extensions in the future!

    ~Device();

    [[nodiscard]] const VkDevice get_device() const {
        assert(device);
        return device;
    }

    [[nodiscard]] const VkPhysicalDevice get_physical_device() const {
        assert(graphics_card);
        return graphics_card;
    }

    [[nodiscard]] const VkQueue get_graphics_queue() const {
        assert(graphics_queue);
        return graphics_queue;
    }

    [[nodiscard]] const VkQueue get_present_queue() const {
        assert(present_queue);
        return present_queue;
    }

    /// @note Transfer queues are the fastest way to copy data across the PCIe bus.
    /// They are heavily underutilized even in modern games.
    /// Transfer queues can be used asynchronously to graphics queuey.
    [[nodiscard]] const VkQueue get_transfer_queue() const {
        assert(transfer_queue);
        return transfer_queue;
    }

    [[nodiscard]] const std::uint32_t get_graphics_queue_family_index() const {
        assert(graphics_queue_family_index.has_value());
        return graphics_queue_family_index.value();
    }

    [[nodiscard]] const std::uint32_t get_present_queue_family_index() const {
        assert(present_queue_family_index.has_value());
        return present_queue_family_index.value();
    }

    [[nodiscard]] const std::uint32_t get_transfer_queue_family_index() const {
        assert(transfer_queue_family_index.has_value());
        return transfer_queue_family_index.value();
    }
};

} // namespace inexor::vulkan_renderer::wrapper
