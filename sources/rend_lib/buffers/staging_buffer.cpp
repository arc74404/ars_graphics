#include "staging_buffer.hpp"

namespace ars_graphics
{
StagingBuffer::StagingBuffer(const LogicalDevice& logical_device,
                             const PhysicalDevice& physical_device)
    : Buffer(logical_device,
             physical_device,
             vk::BufferUsageFlagBits::eTransferSrc,
             vk::MemoryPropertyFlagBits::eHostVisible |
                 vk::MemoryPropertyFlagBits::eHostCoherent)
{
}
} // namespace ars_graphics
