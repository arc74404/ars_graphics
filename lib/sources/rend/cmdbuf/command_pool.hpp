#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "../device/logical_device.hpp"

namespace ars_graphics
{

class Device;

class CommandPool
{
public:
    CommandPool(const LogicalDevice& logical_device,
                const PhysicalDevice& physical_device);

    vk::UniqueCommandBuffer allocateCommandBuffer(
        const LogicalDevice& logical_device) const;

private:
    vk::UniqueCommandPool m_command_pool;
};

} // namespace ars_graphics
