#include "scene.hpp"

#include <algorithm>

#include "instancing/instancer.hpp"

namespace ars_graphics
{
void
Scene::addModel(const Model* new_model)
{
    m_models[new_model]++;
}

namespace
{
void
calculateInds(PerPrimitiveData& per_primitive,
              uint32_t& cur_count_indices,
              const Primitive& primitive)
{
    per_primitive.need_init_in_calc.has_indices =
        (false == primitive.m_indices.empty());

    per_primitive.need_init_in_calc.index_buffer_data_info.first_index =
        std::exchange(cur_count_indices,
                      cur_count_indices + primitive.m_indices.size());

    per_primitive.need_init_in_calc.index_buffer_data_info.index_count =
        primitive.m_indices.size();
}

void
calculateVerts(PerPrimitiveData& per_primitive,
               uint32_t& cur_vertex_offset,
               const Primitive& primitive)
{
    per_primitive.need_init_in_calc.vertex_buffer_data_info.first_vertex =
        std::exchange(cur_vertex_offset,
                      cur_vertex_offset + primitive.m_vertices.vertexOffset());

    per_primitive.need_init_in_calc.vertex_buffer_data_info.vertex_count =
        primitive.m_vertices.vertexOffset();

    per_primitive.need_init_in_calc.index_buffer_data_info.vertex_offset =
        per_primitive.need_init_in_calc.vertex_buffer_data_info.first_vertex;
}

void
calculateInstances(PerPrimitiveData& per_primitive,
                   uint32_t& instance_offset,
                   uint32_t model_instance_count)
{
    per_primitive.need_init_in_calc.vertex_buffer_data_info.first_instance =
        std::exchange(instance_offset, instance_offset + model_instance_count);

    per_primitive.need_init_in_calc.vertex_buffer_data_info.instance_count =
        model_instance_count;

    per_primitive.need_init_in_calc.index_buffer_data_info.first_instance =
        per_primitive.need_init_in_calc.vertex_buffer_data_info.first_instance;

    per_primitive.need_init_in_calc.index_buffer_data_info.instance_count =
        model_instance_count;
}
} // namespace

void
Scene::calcModelInstancing(
    Instancer<ModelInstancingData, StorageBuffer>& model_inst_data,
    size_t count) const
{
    for (size_t i = 0; i < count; ++i)
    {
        model_inst_data.push(ModelInstancingData{1.0});
    }
}

void
Scene::calcMeshInstancing(
    Instancer<MeshInstancingData, StorageBuffer>& model_inst_data,
    const MeshNodeInstancing& mesh_node_instancing,
    size_t mesh_index) const
{
    mesh_node_instancing.fillInstancer(model_inst_data, mesh_index);
}

void
Scene::calcPrimitives(RenderInfo& render_info,
                      const std::vector<Primitive>& primitives,
                      TemperaryInfoCollector& collector,
                      size_t count_model_copies,
                      size_t count_per_this_mesh) const
{
    for (auto&& primitive : primitives)
    {
        PerPrimitiveData per_primitive{
            .need_init_in_calc = {
                .pipeline_choose_info = {
                    primitive.m_vertex_shader_type,
                    primitive.m_vertex_binding_description,
                    primitive.m_vertex_attribute_description}}};

        per_primitive.need_init_in_calc.key_vertex = primitive.m_key_vertex;

        calculateInds(per_primitive, collector.cur_count_indices, primitive);
        calculateVerts(per_primitive, collector.vertex_offset, primitive);
        calculateInstances(per_primitive, collector.instance_count,
                           count_model_copies * count_per_this_mesh);

        per_primitive.need_init_in_calc.primitive_topology =
            primitive.m_primitive_topology;

        per_primitive.need_init_in_calc.material = primitive.m_material;

        primitive.m_vertices.pushData<true>(collector.vertices_data.get());
        pushDataToTheEnd<true>(collector.indices_data.get(),
                               primitive.m_indices);

        render_info.per_primitive_data.emplace_back(std::move(per_primitive));
    }
}

void
Scene::calcMapping(TemperaryInfoCollector& collector,
                   size_t count_models_copies,
                   size_t count_meshes_copies) const
{
    for (uint32_t model_index = collector.models_offset,
                  model_end   = collector.models_offset + count_models_copies;
         model_index < model_end; ++model_index)
    {
        for (uint32_t mesh_index = collector.mesh_offset,
                      mesh_end   = collector.mesh_offset + count_meshes_copies;
             mesh_index < mesh_end; ++mesh_index)
        {
            collector.mapping_data.push({.model_instance_idx = model_index,
                                         .mesh_instance_idx  = mesh_index});
        }
    }
    collector.mesh_offset += count_meshes_copies;
}

void
Scene::fillBuffers(
    const LogicalDevice& logical_device,
    const PhysicalDevice& physical_device,
    const std::vector<MeshNodeInstancing>& mesh_node_instancing_arr,
    RenderInfo& render_info)
{
    TemperaryInfoCollector info_collector;

    uint32_t model_index = 0;
    for (auto&& model : m_models)
    {
        calcModelInstancing(info_collector.model_inst_data, model.second);

        auto&& meshes = model.first->getMeshes();

        const MeshNodeInstancing& mesh_node_instancing =
            mesh_node_instancing_arr[model_index];

        for (size_t mesh_index = 0; mesh_index < meshes.size(); ++mesh_index)
        {
            auto& mesh = meshes[mesh_index];

            if (mesh_node_instancing.noInstanceForThisMesh(mesh_index))
            {

                continue;
            }
            calcMapping(info_collector, model.second,
                        mesh_node_instancing.countInstances(mesh_index));

            calcMeshInstancing(info_collector.mesh_inst_data,
                               mesh_node_instancing, mesh_index);

            calcPrimitives(render_info, mesh.getPrimitives(), info_collector,
                           model.second,
                           mesh_node_instancing.countInstances(mesh_index));
        }
        info_collector.models_offset += model.second;
        ++model_index;
    }

    render_info.per_mesh_instancing =
        info_collector.mesh_inst_data.generateBuffer(logical_device,
                                                     physical_device);

    render_info.instance_mapping = info_collector.mapping_data.generateBuffer(
        logical_device, physical_device);

    render_info.per_model_instancing =
        info_collector.model_inst_data.generateBuffer(logical_device,
                                                      physical_device);

    if (info_collector.indices_data.get().empty() &&
        info_collector.vertices_data.get().empty())
    {
        render_info.m_is_valid = false;
        std::cout << "WARNING: No geometry data to render!" << std::endl;
    }
    else
    {
        render_info.index_buffer = info_collector.indices_data.generateBuffer(
            logical_device, physical_device);

        render_info.vertex_buffer = info_collector.vertices_data.generateBuffer(
            logical_device, physical_device);
    }

    m_models_inst  = std::move(info_collector.model_inst_data.get());
    m_meshes_inst  = std::move(info_collector.mesh_inst_data.get());
    m_mapping_inst = std::move(info_collector.mapping_data.get());
}

std::vector<MeshNodeInstancing>
Scene::calcPerMeshData() const
{
    std::vector<MeshNodeInstancing> res;
    for (auto&& model : m_models)
    {
        MeshNodeInstancing temp;
        temp.initData(model.first->getRoots(), model.first->getNodes(),
                      model.first->getMeshes().size());
        res.emplace_back(std::move(temp));
    }
    return res;
}

void
Scene::bindCamera(const ICamera* cam)
{
    m_camera = cam;
}

RenderInfo
Scene::calculateRenderInfo(const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device)
{
    RenderInfo render_info;

    fillBuffers(logical_device, physical_device, calcPerMeshData(),
                render_info);

    render_info.links.m_model_instancing_data = &m_models_inst;
    render_info.links.m_mesh_instancing_data  = &m_meshes_inst;
    render_info.links.m_mapping               = &m_mapping_inst;
    return render_info;
}

} // namespace ars_graphics
