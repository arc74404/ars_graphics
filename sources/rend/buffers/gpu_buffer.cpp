#include "gpu_buffer.hpp"

#include <iostream>

#include "../../defines.hpp"
#include "../cmdbuf/command_buffer.hpp"

namespace ars_graphics
{
GpuVertexBuffer::GpuVertexBuffer()
    : Buffer(vk::BufferUsageFlagBits::eVertexBuffer |
                 vk::BufferUsageFlagBits::eTransferDst,
             vk::MemoryPropertyFlagBits::eDeviceLocal)
{
}

void
GpuVertexBuffer::bind(vk::CommandBuffer command_buffer) const noexcept
{
    vk::DeviceSize offsets = {0};
    command_buffer.bindVertexBuffers(0, 1, &(get()), &offsets);
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
setData(const LogicalDevice& logical_device,
        const PhysicalDevice& physical_device,
        const void* data,
        vk::DeviceSize byte_size)
{
    CommandBuffer command_buffer{logical_device};

    StagingBuffer staging_buffer;
    FAILED_RESULT_RETURN(staging_buffer.setData(logical_device, physical_device,
                                                data, byte_size),
                         vk::Result::eSuccess)

    ars_graphics::Buffer& this_buffer = *this;

    Buffer::copyBuffer(logical_device, physical_device, staging_buffer,
                       this_buffer, logical_device.getQueue("graphics"),
                       command_buffer.get());

    return vk::Result::eSuccess;
}

}; // namespace ars_graphics
