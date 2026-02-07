#include "buffer.hpp"

#include "../defines.hpp"
#include "../textures/image.hpp"

namespace ars_graphics
{

// const //

vk::Buffer
Buffer::get() const noexcept
{
    return m_buffer.get();
}
vk::DeviceSize
Buffer::getByteSize() const noexcept
{
    return m_byte_size;
}
// ---------- //

Buffer::Buffer(vk::BufferUsageFlags buffer_usage_flags,
               vk::MemoryPropertyFlags requested_properties)
    : m_byte_size(0),
      m_buffer_usage_flags(buffer_usage_flags),
      m_requested_properties(requested_properties)
{
}
vk::Result
Buffer::checkBufferSize(const LogicalDevice& logical_device,
                        const PhysicalDevice& physical_device,
                        vk::DeviceSize required_size)
{
    if (required_size > getByteSize())
    {
        unmap(logical_device);
        return recreate(logical_device, physical_device, required_size);
    }
    return vk::Result::eSuccess;
}

void
Buffer::unmap(const LogicalDevice& logical_device)
{
    if (m_is_mapped && m_memory)
    {
        m_mapped_memory = nullptr;
        m_is_mapped     = false;
        m_byte_size     = 0;
        logical_device.get().unmapMemory(m_memory.get());
    }
}

vk::Result
Buffer::allocateBufferMemory(const LogicalDevice& logical_device,
                             const PhysicalDevice& physical_device)
{
    vk::MemoryRequirements memory_requirements =
        logical_device.get().getBufferMemoryRequirements(m_buffer.get());

    vk::MemoryAllocateInfo alloc_info;
    alloc_info.allocationSize  = memory_requirements.size;
    alloc_info.memoryTypeIndex = physical_device.findMemoryTypeIndex(
        memory_requirements.memoryTypeBits, m_requested_properties);

    FAILED_RESULT_VALUE_RETURN(
        alloc_mem_res, logical_device.get().allocateMemoryUnique(alloc_info),
        vk::Result::eSuccess)

    m_memory = std::move(alloc_mem_res.value);
    FAILED_RESULT_RETURN(logical_device.get().bindBufferMemory(
                             m_buffer.get(), m_memory.get(), 0),
                         vk::Result::eSuccess)

    FAILED_RESULT_VALUE_RETURN(
        map_mem, logical_device.get().mapMemory(m_memory.get(), 0, m_byte_size),
        vk::Result::eSuccess);

    m_mapped_memory = map_mem.value;

    return vk::Result::eSuccess;
}

vk::Result
Buffer::recreate(const LogicalDevice& logical_device,
                 const PhysicalDevice& physical_device,
                 vk::DeviceSize size)
{
    m_byte_size = size;

    vk::BufferCreateInfo buffer_info;
    buffer_info.flags       = vk::BufferCreateFlags();
    buffer_info.size        = size;
    buffer_info.usage       = m_buffer_usage_flags;
    buffer_info.sharingMode = vk::SharingMode::eExclusive;

    FAILED_RESULT_VALUE_RETURN(
        res, logical_device.get().createBufferUnique(buffer_info),
        vk::Result::eSuccess)

    m_buffer = std::move(res.value);

    return allocateBufferMemory(logical_device, physical_device);
}

vk::Result
Buffer::copyBuffer(const LogicalDevice& logical_device,
                   const PhysicalDevice& physical_device,
                   const Buffer& src_buffer,
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

    FAILED_RESULT_RETURN(dst_buffer.checkBufferSize(logical_device,
                                                    physical_device,
                                                    src_buffer.getByteSize()),
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
Buffer::copyBufferToImage(const LogicalDevice& logical_device,
                          const PhysicalDevice& physical_device,
                          const Buffer& src_buffer,
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

    dst_image.copyBufferToImage(src_buffer.get(), command_buffer, region);

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
Buffer::setData(const LogicalDevice& logical_device,
                const PhysicalDevice& physical_device,
                const void* data,
                vk::DeviceSize byte_size)
{
    FAILED_RESULT_RETURN(
        checkBufferSize(logical_device, physical_device, byte_size),
        vk::Result::eSuccess);

    memcpy(m_mapped_memory, data, byte_size);

    return vk::Result::eSuccess;
}
}; // namespace ars_graphics
