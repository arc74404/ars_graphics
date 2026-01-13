#include "gpu_buffer.hpp"

#include <iostream>

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
GpuVertexBuffer::bind(const vk::CommandBuffer& command_buffer) const
{
    vk::DeviceSize offsets = {0};
    command_buffer.bindVertexBuffers(0, 1, &(buffer()), &offsets);
}

void
GpuVertexBuffer::draw(const vk::CommandBuffer& command_buffer,
                      uint32_t vertex_count,
                      uint32_t instance_count,
                      uint32_t first_vertex,
                      uint32_t first_instance) const
{
    command_buffer.draw(vertex_count, instance_count, first_vertex,
                        first_instance);
}

bool
GpuVertexBuffer::setData(const LogicalDevice& logical_device,
                         const PhysicalDevice& physical_device,
                         const void* data,
                         const vk::DeviceSize& byte_size)
{
    CommandBuffer command_buffer{logical_device};

    StagingBuffer staging_buffer;
    bool check = staging_buffer.setData(logical_device, physical_device, data,
                                        byte_size);
    if (check == false)
    {
        return false;
    }

    Buffer::copyBuffer(logical_device, physical_device, &staging_buffer, this,
                       logical_device.getQueue("graphics"),
                       command_buffer.get());
    return true;
}

}; // namespace ars_graphics
