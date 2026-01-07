#include "storage_buffer.hpp"

namespace ars_graphics
{
StorageBuffer::StorageBuffer()
    : Buffer(vk::BufferUsageFlagBits::eStorageBuffer,
             vk::MemoryPropertyFlagBits::eHostVisible |
                 vk::MemoryPropertyFlagBits::eHostCoherent)
{
}
} // namespace ars_graphics
