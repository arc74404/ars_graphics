#include "command_buffer.hpp"

#include "command_pool.hpp"

namespace ars_graphics
{

std::unique_ptr<CommandPool> CommandBuffer::m_shared_command_pool = nullptr;

void
CommandBuffer::createCommandPool(const LogicalDevice& ls,
                                 const PhysicalDevice& phd)
{
    m_shared_command_pool = std::make_unique<CommandPool>(ls, phd);
}

CommandBuffer::CommandBuffer(const LogicalDevice& device)
{
    if (m_shared_command_pool.get() == nullptr)
    {
        throw std::logic_error(
            "Failed to create command before because pool was not created. "
            "Call static method createCommandPool before construct "
            "CommandBuffers.");
    }
    m_command_buffer = m_shared_command_pool->allocateCommandBuffer(device);
}

const vk::CommandBuffer&
CommandBuffer::get() const noexcept
{
    return m_command_buffer.get();
}

void
CommandBuffer::destroyCommandPool()
{
    m_shared_command_pool->destroy();
}

void
CommandBuffer::destroy()
{
    m_command_buffer.reset();
}

} // namespace ars_graphics
