#pragma once

#include "../buffers/gpu_buffer.hpp"
#include "../buffers/index_buffer.hpp"
#include "../buffers/storage_buffer.hpp"
#include "../mesh/primitive.hpp"
#include "../pipelines/pipeline_layout_storage.hpp"
#include "../shaders/shaders_data_structs/instance_data.hpp"

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

struct PipelineChooseInfo
{
    PipelineChooseInfo(
        ShaderType sht,
        const vk::VertexInputBindingDescription& vbd,
        const std::vector<vk::VertexInputAttributeDescription>& vad)
        : vertex_shader_type(sht),
          vertex_binding_description(vbd),
          vertex_attribute_description(vad)
    {
    }
    ShaderType vertex_shader_type;
    const vk::VertexInputBindingDescription& vertex_binding_description;
    const std::vector<vk::VertexInputAttributeDescription>&
        vertex_attribute_description;
};

struct PrimitiveNeedInitInRenderInfoCalculateing
{
    const Material* material;
    PipelineChooseInfo pipeline_choose_info;
    IndexBufferDrawInfo index_buffer_data_info;
    VertexBufferDrawInfo vertex_buffer_data_info;
    bool has_indices;
    vk::PrimitiveTopology primitive_topology;
    uint64_t key_vertex;
};

struct PerPrimitiveData
{
    PrimitiveNeedInitInRenderInfoCalculateing need_init_in_calc;
    const Pipeline* pipeline;
};

struct PerMeshInstancingData
{
    std::vector<MeshInstancingData> m_data;
};

struct PerModelInstancingData
{
    std::vector<ModelInstancingData> m_data;
};

struct InstanceMapping
{
    uint32_t model_instance_idx;
    uint32_t mesh_instance_idx;
};

struct RenderInfo
{
    bool m_is_valid = true;
    std::vector<PerPrimitiveData> per_primitive_data;

    StorageBuffer per_model_instancing;
    StorageBuffer per_mesh_instancing;

    StorageBuffer instance_mapping;

    GpuVertexBuffer vertex_buffer;
    IndexBuffer index_buffer;
};
} // namespace ars_graphics
