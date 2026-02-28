#include "storage_buffer.hpp"

namespace ars_graphics
{
StorageBuffer::StorageBuffer(const LogicalDevice& logical_device,
                         const PhysicalDevice& physical_device)
    : Buffer(logical_device,
             physical_device,
             vk::BufferUsageFlagBits::eStorageBuffer,
             vk::MemoryPropertyFlagBits::eHostVisible |
                 vk::MemoryPropertyFlagBits::eHostCoherent)
{
}
} // namespace ars_graphics
