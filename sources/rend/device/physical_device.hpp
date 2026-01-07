#pragma once

#include <optional>

#include "../instance/instance.hpp"

namespace ars_graphics
{
struct QueueFamilyIndices
{
    bool isComplete();

    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;
};

struct SwapChainSupportDetails
{
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> present_modes;
};

class PhysicalDevice
{
public:
    PhysicalDevice(const Instance& instance, const vk::SurfaceKHR& surface);

    QueueFamilyIndices getQueueFamilyIndices() const;

    uint32_t findMemoryTypeIndex(
        uint32_t supported_memory_indices,
        vk::MemoryPropertyFlags requested_properties) const;

    SwapChainSupportDetails getSurfaceSupportDetails(
        const vk::SurfaceKHR& surface) const;

    const vk::PhysicalDevice& get() const;

private:
    void choosePhysicalDevice(const Instance& instance,
                              const vk::SurfaceKHR& surface);

    QueueFamilyIndices findQueueFamilies(const vk::SurfaceKHR& surface) const;

    static bool isSuitable(const vk::PhysicalDevice& device);

    static bool checkDeviceExtensionSupport(
        const vk::PhysicalDevice& device,
        const std::vector<const char*>& requested_extensions);

    static bool supportMailBox(const vk::PhysicalDevice& device,
                               const vk::SurfaceKHR& surface);

    vk::PhysicalDevice m_physical_device;

    QueueFamilyIndices m_queue_family_indices;
};
} // namespace ars_graphics
