#include "uniform_buffer.hpp"

namespace ars_graphics
{
UniformBuffer::UniformBuffer(const LogicalDevice& logical_device,
                             const PhysicalDevice& physical_device)
    : Buffer(logical_device,
             physical_device,
             vk::BufferUsageFlagBits::eUniformBuffer,
             vk::MemoryPropertyFlagBits::eHostVisible |
                 vk::MemoryPropertyFlagBits::eHostCoherent)
{
}
} // namespace ars_graphics
