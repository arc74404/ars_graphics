#include "buffer.hpp"

#include "../textures/image.hpp"

namespace ars_graphics
{

Buffer::Buffer(vk::BufferUsageFlags buffer_usage_flags,
               vk::MemoryPropertyFlags requested_properties)
    : m_byte_size(0),
      m_buffer_usage_flags(buffer_usage_flags),
      m_requested_properties(requested_properties)
{
}

const vk::Buffer&
Buffer::buffer() const noexcept
{
    return m_buffer.get();
}

void
Buffer::checkBufferSize(const LogicalDevice& logical_device,
                        const PhysicalDevice& physical_device,
                        const vk::DeviceSize& required_size)
{
    if (required_size > Buffer::byteSize())
    {
        Buffer::recreate(logical_device, physical_device, required_size);
    }
}

void
Buffer::allocateBufferMemory(const LogicalDevice& logical_device,
                             const PhysicalDevice& physical_device)
{
    vk::MemoryRequirements memory_requirements =
        logical_device.get().getBufferMemoryRequirements(m_buffer.get());

    vk::MemoryAllocateInfo allocInfo;
    allocInfo.allocationSize  = memory_requirements.size;
    allocInfo.memoryTypeIndex = physical_device.findMemoryTypeIndex(
        memory_requirements.memoryTypeBits, m_requested_properties);

    auto&& res = logical_device.get().allocateMemoryUnique(allocInfo);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to allocate buffer nenory");
    }

    m_memory = std::move(res.value);
    logical_device.get().bindBufferMemory(m_buffer.get(), m_memory.get(), 0);
}

void
Buffer::recreate(const LogicalDevice& logical_device,
                 const PhysicalDevice& physical_device,
                 const vk::DeviceSize& size)
{
    m_byte_size = size;

    vk::BufferCreateInfo bufferInfo;
    bufferInfo.flags       = vk::BufferCreateFlags();
    bufferInfo.size        = size;
    bufferInfo.usage       = m_buffer_usage_flags;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    auto&& res = logical_device.get().createBufferUnique(bufferInfo);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed to recreate buffer");
    }
    m_buffer = std::move(res.value);

    allocateBufferMemory(logical_device, physical_device);
}

vk::DeviceSize
Buffer::byteSize() const noexcept
{
    return m_byte_size;
}

void
Buffer::copyBuffer(const LogicalDevice& logical_device,
                   const PhysicalDevice& physical_device,
                   const Buffer* const src_buffer,
                   Buffer* const dst_buffer,
                   vk::Queue queue,
                   const vk::CommandBuffer& command_buffer)
{
    command_buffer.reset();

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    command_buffer.begin(beginInfo);

    vk::BufferCopy copyRegion;
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size      = src_buffer->m_byte_size;

    dst_buffer->checkBufferSize(logical_device, physical_device,
                                src_buffer->m_byte_size);
    command_buffer.copyBuffer(src_buffer->m_buffer.get(),
                              dst_buffer->m_buffer.get(), 1, &copyRegion);

    command_buffer.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &command_buffer;
    queue.submit(1, &submitInfo, nullptr);
    queue.waitIdle();
}

void
Buffer::copyBufferToImage(const LogicalDevice& logical_device,
                          const PhysicalDevice& physical_device,
                          const Buffer* const src_buffer,
                          Image& dst_image,
                          vk::Queue queue,
                          const vk::CommandBuffer& command_buffer,
                          float width,
                          float height)
{
    command_buffer.reset();

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    command_buffer.begin(beginInfo);

    vk::ImageMemoryBarrier barrier = dst_image.memoryBarrier();

    command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                   vk::PipelineStageFlagBits::eTransfer, {}, 0,
                                   nullptr, 0, nullptr, 1, &barrier);

    vk::BufferImageCopy region(
        0, 0, 0, {vk::ImageAspectFlagBits::eColor, 0, 0, 1}, {0, 0, 0},
        {static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1});

    dst_image.getDataFromBuffer(src_buffer->buffer(), command_buffer, region);

    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
    barrier.oldLayout     = vk::ImageLayout::eTransferDstOptimal;
    barrier.newLayout     = vk::ImageLayout::eShaderReadOnlyOptimal;

    command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                   vk::PipelineStageFlagBits::eFragmentShader,
                                   {}, 0, nullptr, 0, nullptr, 1, &barrier);

    command_buffer.end();
}

bool
Buffer::setData(const LogicalDevice& logical_device,
                const PhysicalDevice& physical_device,
                void* data,
                const vk::DeviceSize& byte_size)
{
    checkBufferSize(logical_device, physical_device, byte_size);

    auto&& res = logical_device.get().mapMemory(m_memory.get(), 0, m_byte_size);

    if (res.result != vk::Result::eSuccess)
    {
        return false;
    }

    void* memory_location = res.value;
    memcpy(memory_location, data, m_byte_size);

    logical_device.get().unmapMemory(m_memory.get());

    return true;
}
}; // namespace ars_graphics
