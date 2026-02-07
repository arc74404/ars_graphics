#pragma once
#include "../cmdbuf/command_buffer.hpp"

#include "buffer.hpp"
#include "staging_buffer.hpp"
#include "storage_buffer.hpp"

namespace ars_graphics
{

class GpuVertexBuffer final : public Buffer
{
public:
    GpuVertexBuffer();

    void bind(vk::CommandBuffer command_buffer) const noexcept;

    void draw(vk::CommandBuffer command_buffer,
              uint32_t vertex_count,
              uint32_t instance_count,
              uint32_t first_vertex,
              uint32_t first_instance) const noexcept;

    vk::Result setData(const LogicalDevice& logical_device,
                       const PhysicalDevice& physical_device,
                       const void* data,
                       vk::DeviceSize byte_size) override;

private:
};
}; // namespace ars_graphics
