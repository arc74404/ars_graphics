#pragma once

#include <map>

#include "buffers/gpu_buffer.hpp"
#include "buffers/index_buffer.hpp"
#include "instancing/instancer.hpp"
#include "instancing/mesh_node_instancing.hpp"
#include "model/model_manager.hpp"
#include "render/renderable.hpp"
#include "shaders/shaders_data_structs/instance_data.hpp"

namespace ars_graphics
{

class Renderer;

struct TemperaryInfoCollector
{
    uint32_t cur_count_indices = 0;
    uint32_t vertex_offset     = 0; // not bytes, count vertex
    uint32_t instance_count    = 0;

    size_t models_offset = 0;
    size_t mesh_offset   = 0;

    Instancer<float, GpuVertexBuffer> vertices_data;
    Instancer<uint32_t, IndexBuffer> indices_data;

    Instancer<MeshInstancingData, StorageBuffer> mesh_inst_data;
    Instancer<ModelInstancingData, StorageBuffer> model_inst_data;
    Instancer<InstanceMapping, StorageBuffer> mapping_data;
};

class Scene final : public Renderable
{
public:
    Scene() = default;

    void addModel(const Model* new_model);

    RenderInfo calculateRenderInfo(
        const LogicalDevice& logical_device,
        const PhysicalDevice& physical_device) const override;

private:
    void fillBuffers(
        const LogicalDevice& logical_device,
        const PhysicalDevice& physical_device,
        const std::vector<MeshNodeInstancing>& mesh_node_instancing_arr,
        RenderInfo& render_info) const;

    std::vector<MeshNodeInstancing> calcPerMeshData() const;

    void calcModelInstancing(
        Instancer<ModelInstancingData, StorageBuffer>& model_inst_data,
        size_t count) const;

    void calcMeshInstancing(
        Instancer<MeshInstancingData, StorageBuffer>& model_inst_data,
        const MeshNodeInstancing& mesh_node_instancing,
        size_t mesh_index) const;

    void calcPrimitives(RenderInfo& render_info,
                        const std::vector<Primitive>& primitives,
                        TemperaryInfoCollector& collector,
                        size_t count_model_copies,
                        size_t count_per_this_mesh) const;

    void calcMapping(TemperaryInfoCollector& collector,
                     size_t count_models_copies,
                     size_t count_meshes_copies) const;

    std::map<const Model*, size_t> m_models;
};
} // namespace ars_graphics
