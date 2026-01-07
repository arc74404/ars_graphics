#include "staging_buffer.hpp"

namespace ars_graphics
{
StagingBuffer::StagingBuffer()
    : Buffer(vk::BufferUsageFlagBits::eTransferSrc,
             vk::MemoryPropertyFlagBits::eHostVisible |
                 vk::MemoryPropertyFlagBits::eHostCoherent)
{
}
} // namespace ars_graphics
