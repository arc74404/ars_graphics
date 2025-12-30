#pragma once

#include <map>

#include "physical_device.hpp"

namespace ars_graphics
{

struct QueueData
{
    uint32_t family_index;
    uint32_t queue_index;

    float priority;
};

class LogicalDevice
{
public:
    LogicalDevice() = default;

    void create(const vk::PhysicalDevice& physical_device,
                const QueueFamilyIndices& queue_family_indices);

    vk::Queue getQueue(const std::string& queue_name) const;

    const vk::Device& get() const;

private:
    vk::PhysicalDeviceFeatures setupDeviceFeatures();

    std::vector<vk::DeviceQueueCreateInfo> getQueuesInfo();

    void setupQueues(const QueueFamilyIndices& queue_family_indices);

    std::map<std::string, QueueData> m_queues;

    vk::UniqueDevice m_logical_device;
};
} // namespace ars_graphics
