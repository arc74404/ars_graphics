#include "scene.hpp"

#include <algorithm>

namespace ars_graphics
{
void
Scene::addModel(const Model* new_model)
{
    m_models[new_model]++;
}

Scene::Scene()
{
}

namespace
{
void
calculateInds(PerPrimitiveData& per_primitive,
              uint32_t& cur_count_indices,
              const Primitive& primitive)
{
    per_primitive.has_indices = (false == primitive.m_indices.empty());

    per_primitive.index_buffer_data_info.first_index = std::exchange(
        cur_count_indices, cur_count_indices + primitive.m_indices.size());

    per_primitive.index_buffer_data_info.index_count =
        primitive.m_indices.size();
}

void
calculateVerts(PerPrimitiveData& per_primitive,
               uint32_t& cur_vertex_offset,
               const Primitive& primitive)
{
    per_primitive.vertex_buffer_data_info.first_vertex =
        std::exchange(cur_vertex_offset,
                      cur_vertex_offset + primitive.m_vertices.vertexOffset());

    per_primitive.vertex_buffer_data_info.vertex_count =
        primitive.m_vertices.vertexOffset();

    per_primitive.index_buffer_data_info.vertex_offset =
        per_primitive.vertex_buffer_data_info.first_vertex;
}

void
calculateInstances(PerPrimitiveData& per_primitive,
                   uint32_t& instance_offset,
                   uint32_t model_instance_count)
{
    per_primitive.vertex_buffer_data_info.first_instance =
        std::exchange(instance_offset, instance_offset + model_instance_count);

    per_primitive.vertex_buffer_data_info.instance_count = model_instance_count;

    per_primitive.index_buffer_data_info.first_instance =
        per_primitive.vertex_buffer_data_info.first_instance;

    per_primitive.index_buffer_data_info.instance_count = model_instance_count;
}
} // namespace

RenderInfo
Scene::calculateRenderInfo(const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device) const
{
    RenderInfo render_info;

    uint32_t cur_count_indices = 0;
    uint32_t vertex_offset     = 0; // not bytes, count vertex
    uint32_t instance_count    = 0;

    std::vector<char> vertices_data;
    std::vector<uint32_t> indices_data;

    for (auto&& model : m_models)
    {
        for (auto&& mesh : model.first->getMeshes())
        {
            for (auto&& primitive : mesh.getPrimitives())
            {
                PerPrimitiveData per_primitive;

                calculateInds(per_primitive, cur_count_indices, primitive);
                calculateVerts(per_primitive, vertex_offset, primitive);
                calculateInstances(per_primitive, instance_count, model.second);

                per_primitive.primitive_topology =
                    primitive.m_primitive_topology;

                per_primitive.material = primitive.m_material;
                per_primitive.pipeline = primitive.m_pipeline;

                primitive.m_vertices.pushData<true>(vertices_data);

                primitive.m_vertices.pushData<true>(vertices_data);
                pushDataToTheEnd<true>(indices_data, primitive.m_indices);

                render_info.per_primitive_data.emplace_back(
                    std::move(per_primitive));
            }
        }
    }
    render_info.index_buffer.setData(logical_device, physical_device,
                                     indices_data.data(),
                                     indices_data.size() * sizeof(uint32_t));

    render_info.vertex_buffer.setData(logical_device, physical_device,
                                      vertices_data.data(),
                                      vertices_data.size());

    return render_info;
}

} // namespace ars_graphics
