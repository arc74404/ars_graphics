#ifndef GPU_BUFFER_HPP
#define GPU_BUFFER_HPP

#include "../cmdbuf/command_buffer.hpp

#include "buffer.hpp"
#include "staging_buffer.hpp"
#include "storage_buffer.hpp"

namespace ars_graphics
{

class GpuVertexBuffer : public Buffer
{
public:
    GpuVertexBuffer();

    void bind(const vk::CommandBuffer& command_buffer) const;

    bool setData(const LogicalDevice& logical_device,
                 const PhysicalDevice& physical_device,
                 void* data,
                 const vk::DeviceSize& byte_size) override;

private:
};
}; // namespace ars_graphics

#endif // GPU_BUFFER_HPP
