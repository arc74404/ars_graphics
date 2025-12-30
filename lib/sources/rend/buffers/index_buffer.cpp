#include "index_buffer.hpp"

namespace ars_graphics
{
IndexBuffer::IndexBuffer()
    : Buffer(vk::BufferUsageFlagBits::eIndexBuffer,
             vk::MemoryPropertyFlagBits::eHostVisible |
                 vk::MemoryPropertyFlagBits::eHostCoherent)
{
}

void
IndexBuffer::bind(const vk::CommandBuffer& command_buffer) const
{
    command_buffer.bindIndexBuffer(buffer(), 0, vk::IndexType::eUint32);
}

void
IndexBuffer::draw(const vk::CommandBuffer& command_buffer,
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
