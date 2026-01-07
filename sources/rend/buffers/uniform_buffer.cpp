#include "uniform_buffer.hpp"

namespace ars_graphics
{
UniformBuffer::UniformBuffer()
    : Buffer(vk::BufferUsageFlagBits::eUniformBuffer,
             vk::MemoryPropertyFlagBits::eHostVisible |
                 vk::MemoryPropertyFlagBits::eHostCoherent)
{
}
} // namespace ars_graphics
