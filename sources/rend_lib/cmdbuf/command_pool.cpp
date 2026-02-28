#include "command_pool.hpp"

#include <iostream>

#include "../device/logical_device.hpp"

namespace ars_graphics
{

CommandPool::CommandPool(vk::Device logical_device,
                         const PhysicalDevice& physical_device)
{
    auto queue_family_indices = physical_device.getQueueFamilyIndices();

    vk::CommandPoolCreateInfo pool_info;
    pool_info.flags = vk::CommandPoolCreateFlags() |
                      vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    pool_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

    auto&& res = logical_device.createCommandPoolUnique(pool_info);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to create CommandPool");
    }
    m_command_pool = std::move(res.value);
}

std::optional<vk::UniqueCommandBuffer>
CommandPool::allocateCommandBuffer(vk::Device logical_device) const
{
    vk::CommandBufferAllocateInfo alloc_info = {};
    alloc_info.commandPool                   = m_command_pool.get();
    alloc_info.level                         = vk::CommandBufferLevel::ePrimary;
    alloc_info.commandBufferCount            = 1;

    auto&& res = logical_device.allocateCommandBuffersUnique(alloc_info);

    if (res.result != vk::Result::eSuccess)
    {
        return std::nullopt;
    }
    return std::move(res.value[0]);
}

void
CommandPool::destroy()
{
    m_command_pool.reset();
}

} // namespace ars_graphics
