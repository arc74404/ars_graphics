#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "command_pool.hpp"

namespace ars_graphics
{
class CommandBuffer
{
public:
    CommandBuffer(const LogicalDevice& device);

    static void createCommandPool(const LogicalDevice& ls,
                                  const PhysicalDevice& phd);

    const vk::CommandBuffer& get() const noexcept;

private:
    static std::unique_ptr<CommandPool> m_shared_command_pool;

    vk::UniqueCommandBuffer m_command_buffer;
};
} // namespace ars_graphics
