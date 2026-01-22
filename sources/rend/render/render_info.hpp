#pragma once

#include "../buffers/gpu_buffer.hpp"
#include "../buffers/index_buffer.hpp"
#include "../pipelines/pipeline_layout_storage.hpp"

namespace ars_graphics
{
struct IndexBufferDrawInfo
{
    uint32_t index_count;
    uint32_t instance_count;
    uint32_t first_index;
    uint32_t vertex_offset;
    uint32_t first_instance;
};
struct VertexBufferDrawInfo
{
    uint32_t vertex_count;
    uint32_t instance_count;
    uint32_t first_vertex;
    uint32_t first_instance;
};

struct PerPrimitiveData
{
    const Pipeline* pipeline;
    const Material* material;
    IndexBufferDrawInfo index_buffer_data_info;
    VertexBufferDrawInfo vertex_buffer_data_info;
    bool has_indices;
    vk::PrimitiveTopology primitive_topology;
};

struct RenderInfo
{
    bool m_is_valid = true;
    std::vector<PerPrimitiveData> per_primitive_data;

    GpuVertexBuffer vertex_buffer;
    IndexBuffer index_buffer;
};
} // namespace ars_graphics
