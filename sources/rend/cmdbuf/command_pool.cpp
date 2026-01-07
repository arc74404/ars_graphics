#include "command_pool.hpp"

#include <iostream>

#include "../device/logical_device.hpp"

namespace ars_graphics
{

CommandPool::CommandPool(const LogicalDevice& logical_device,
                         const PhysicalDevice& physical_device)
{
    auto queue_family_indices = physical_device.getQueueFamilyIndices();

    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.flags = vk::CommandPoolCreateFlags() |
                     vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    poolInfo.queueFamilyIndex = queue_family_indices.graphics_family.value();

    auto&& res = logical_device.get().createCommandPoolUnique(poolInfo);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to create CommandPool");
    }
    m_command_pool = std::move(res.value);
}

vk::UniqueCommandBuffer
CommandPool::allocateCommandBuffer(const LogicalDevice& logical_device) const
{
    vk::CommandBufferAllocateInfo alloc_info = {};
    alloc_info.commandPool                   = m_command_pool.get();
    alloc_info.level                         = vk::CommandBufferLevel::ePrimary;
    alloc_info.commandBufferCount            = 1;

    auto&& res = logical_device.get().allocateCommandBuffersUnique(alloc_info);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to allocate cmdbuf");
    }
    return std::move(res.value[0]);
}
} // namespace ars_graphics
