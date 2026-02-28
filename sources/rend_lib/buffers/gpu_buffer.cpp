#include "gpu_buffer.hpp"

#include <iostream>

namespace ars_graphics
{
GpuVertexBuffer::GpuVertexBuffer(const LogicalDevice& logical_device,
                                 const PhysicalDevice& physical_device,
                                 vk::CommandBuffer cmd)
    : Buffer(logical_device,
             physical_device,
             vk::BufferUsageFlagBits::eVertexBuffer |
                 vk::BufferUsageFlagBits::eTransferDst,
             vk::MemoryPropertyFlagBits::eDeviceLocal),
      m_cmd(cmd)
{
}

void
GpuVertexBuffer::bind(vk::CommandBuffer command_buffer) const noexcept
{
    vk::DeviceSize offsets = {0};
    vk::Buffer buf         = *this;
    command_buffer.bindVertexBuffers(0, 1, &buf, &offsets);
}

void
GpuVertexBuffer::draw(vk::CommandBuffer command_buffer,
                      uint32_t vertex_count,
                      uint32_t instance_count,
                      uint32_t first_vertex,
                      uint32_t first_instance) const noexcept
{
    command_buffer.draw(vertex_count, instance_count, first_vertex,
                        first_instance);
}

vk::Result
GpuVertexBuffer::setDataImpl(const void* data, vk::DeviceSize byte_size)
{

    StagingBuffer staging_buffer(m_logical_device, m_physical_device);
    FAILED_RESULT_RETURN(staging_buffer.setData(data, byte_size),
                         vk::Result::eSuccess)

    ars_graphics::Buffer& this_buffer = *this;

    Buffer::copyBuffer(staging_buffer, this_buffer,
                       m_logical_device.getQueue("graphics"), m_cmd);

    return vk::Result::eSuccess;
}

}; // namespace ars_graphics
