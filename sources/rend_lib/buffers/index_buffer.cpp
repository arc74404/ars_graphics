#include "index_buffer.hpp"

namespace ars_graphics
{
IndexBuffer::IndexBuffer(const LogicalDevice& logical_device,
                         const PhysicalDevice& physical_device)
    : Buffer(logical_device,
             physical_device,
             vk::BufferUsageFlagBits::eIndexBuffer,
             vk::MemoryPropertyFlagBits::eHostVisible |
                 vk::MemoryPropertyFlagBits::eHostCoherent)
{
}

void
IndexBuffer::bind(vk::CommandBuffer command_buffer) const
{
    vk::Buffer buf = *this;
    command_buffer.bindIndexBuffer(*this, 0, vk::IndexType::eUint32);
}

void
IndexBuffer::draw(vk::CommandBuffer command_buffer,
                  uint32_t index_count,
                  uint32_t instance_count,
                  uint32_t first_index,
                  uint32_t vertex_offset,
                  uint32_t first_instance) const
{
    command_buffer.drawIndexed(index_count, instance_count, first_index,
                               vertex_offset, first_instance);
}
} // namespace ars_graphics
