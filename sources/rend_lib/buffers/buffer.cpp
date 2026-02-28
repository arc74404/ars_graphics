#include "buffer.hpp"

#include "../textures/image.hpp"

namespace ars_graphics
{

// const //
Buffer::
operator vk::Buffer() const noexcept
{
    return m_buffer.get();
}
vk::DeviceSize
Buffer::getByteSize() const noexcept
{
    return m_byte_size;
}
// ---------- //

Buffer::Buffer(const LogicalDevice& logical_device,
               const PhysicalDevice& m_physical_device,
               vk::BufferUsageFlags buffer_usage_flags,
               vk::MemoryPropertyFlags requested_properties)
    : m_logical_device(logical_device),
      m_physical_device(m_physical_device),
      m_byte_size(0),
      m_buffer_usage_flags(buffer_usage_flags),
      m_requested_properties(requested_properties)
{
}

vk::Result
Buffer::checkBufferSize(vk::DeviceSize required_size)
{
    if (required_size > getByteSize())
    {
        unmap();
        return recreate(required_size);
    }
    return vk::Result::eSuccess;
}

void
Buffer::unmap() noexcept
{
    if (m_is_mapped && m_memory)
    {
        m_mapped_memory = nullptr;
        m_is_mapped     = false;
        m_byte_size     = 0;
        vk::Device(m_logical_device).unmapMemory(m_memory.get());
    }
}

vk::Result
Buffer::allocateBufferMemory()
{
    vk::MemoryRequirements memory_requirements =
        vk::Device(m_logical_device)
            .getBufferMemoryRequirements(m_buffer.get());

    vk::MemoryAllocateInfo alloc_info;
    alloc_info.allocationSize  = memory_requirements.size;
    alloc_info.memoryTypeIndex = m_physical_device.findMemoryTypeIndex(
        memory_requirements.memoryTypeBits, m_requested_properties);

    FAILED_RESULT_VALUE_RETURN(
        alloc_mem_res,
        vk::Device(m_logical_device).allocateMemoryUnique(alloc_info),
        vk::Result::eSuccess)

    m_memory = std::move(alloc_mem_res.value);
    FAILED_RESULT_RETURN(
        vk::Device(m_logical_device)
            .bindBufferMemory(m_buffer.get(), m_memory.get(), 0),
        vk::Result::eSuccess)

    FAILED_RESULT_VALUE_RETURN(
        map_mem,
        vk::Device(m_logical_device).mapMemory(m_memory.get(), 0, m_byte_size),
        vk::Result::eSuccess);

    m_mapped_memory = map_mem.value;

    return vk::Result::eSuccess;
}

vk::Result
Buffer::recreate(vk::DeviceSize size)
{
    m_byte_size = size;

    vk::BufferCreateInfo buffer_info;
    buffer_info.flags       = vk::BufferCreateFlags();
    buffer_info.size        = size;
    buffer_info.usage       = m_buffer_usage_flags;
    buffer_info.sharingMode = vk::SharingMode::eExclusive;

    FAILED_RESULT_VALUE_RETURN(
        res, vk::Device(m_logical_device).createBufferUnique(buffer_info),
        vk::Result::eSuccess)

    m_buffer = std::move(res.value);

    return allocateBufferMemory();
}

vk::Result
Buffer::copyBuffer(const Buffer& src_buffer,
                   Buffer& dst_buffer,
                   vk::Queue queue,
                   vk::CommandBuffer command_buffer)
{
    FAILED_RESULT_RETURN(command_buffer.reset(), vk::Result::eSuccess)

    vk::CommandBufferBeginInfo begin_info;
    begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

    FAILED_RESULT_RETURN(command_buffer.begin(begin_info), vk::Result::eSuccess)

    vk::BufferCopy copy_region;
    copy_region.srcOffset = 0;
    copy_region.dstOffset = 0;
    copy_region.size      = src_buffer.getByteSize();

    FAILED_RESULT_RETURN(dst_buffer.checkBufferSize(src_buffer.getByteSize()),
                         vk::Result::eSuccess)

    command_buffer.copyBuffer(src_buffer.m_buffer.get(),
                              dst_buffer.m_buffer.get(), 1, &copy_region);

    FAILED_RESULT_RETURN(command_buffer.end(), vk::Result::eSuccess)

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &command_buffer;

    FAILED_RESULT_RETURN(queue.submit(1, &submitInfo, nullptr),
                         vk::Result::eSuccess)
    return queue.waitIdle();
}

vk::Result
Buffer::copyBufferToImage(const Buffer& src_buffer,
                          Image& dst_image,
                          vk::Queue queue,
                          vk::CommandBuffer command_buffer,
                          uint32_t width,
                          uint32_t height)
{
    FAILED_RESULT_RETURN(command_buffer.reset(), vk::Result::eSuccess)

    vk::CommandBufferBeginInfo begin_info;
    begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    FAILED_RESULT_RETURN(command_buffer.begin(begin_info), vk::Result::eSuccess)

    vk::ImageMemoryBarrier barrier = dst_image.memoryBarrier();

    command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                   vk::PipelineStageFlagBits::eTransfer, {}, 0,
                                   nullptr, 0, nullptr, 1, &barrier);

    vk::BufferImageCopy region(0, 0, 0,
                               {vk::ImageAspectFlagBits::eColor, 0, 0, 1},
                               {0, 0, 0}, {width, height, 1});

    dst_image.copyBufferToImage(src_buffer, command_buffer, region);

    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
    barrier.oldLayout     = vk::ImageLayout::eTransferDstOptimal;
    barrier.newLayout     = vk::ImageLayout::eShaderReadOnlyOptimal;

    command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                   vk::PipelineStageFlagBits::eFragmentShader,
                                   {}, 0, nullptr, 0, nullptr, 1, &barrier);

    FAILED_RESULT_RETURN(command_buffer.end(), vk::Result::eSuccess)

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &command_buffer;

    FAILED_RESULT_RETURN(queue.submit(1, &submitInfo, nullptr),
                         vk::Result::eSuccess)
    return queue.waitIdle();
}

vk::Result
Buffer::setData(const void* data, vk::DeviceSize byte_size)
{
    return setDataImpl(data, byte_size);
}

vk::Result
Buffer::setDataImpl(const void* data, vk::DeviceSize byte_size)
{
    FAILED_RESULT_RETURN(checkBufferSize(byte_size), vk::Result::eSuccess);

    memcpy(m_mapped_memory, data, byte_size);

    return vk::Result::eSuccess;
}
}; // namespace ars_graphics
