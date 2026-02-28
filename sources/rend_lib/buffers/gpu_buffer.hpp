#pragma once

#include "buffer.hpp"
#include "staging_buffer.hpp"
#include "storage_buffer.hpp"

namespace ars_graphics
{

class GpuVertexBuffer final : public Buffer
{
public:
    GpuVertexBuffer(const LogicalDevice& logical_device,
                    const PhysicalDevice& physical_device,
                    vk::CommandBuffer cmd);

    void bind(vk::CommandBuffer command_buffer) const noexcept;

    void draw(vk::CommandBuffer command_buffer,
              uint32_t vertex_count,
              uint32_t instance_count,
              uint32_t first_vertex,
              uint32_t first_instance) const noexcept;

private:
    vk::Result setDataImpl(const void* data, vk::DeviceSize byte_size) override;

    vk::CommandBuffer m_cmd;
};
}; // namespace ars_graphics
