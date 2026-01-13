#pragma once

#include "../pipelines/pipeline.hpp"
#include "../pipelines/pipeline_manager.hpp"
#include "../vertex/vertex_data.hpp"

namespace ars_graphics
{
struct Primitive final
{
    template <typename... VertexAttributes>
    Primitive(PipelineManager& manager,
              const vk::RenderPass& render_pass,
              std::vector<uint32_t>&& indices,
              const std::vector<Vertex<VertexAttributes...>>& vertices,
              vk::PrimitiveTopology primitive_topology,
              const Material* material)
        : m_pipeline(manager.getPipeline<Vertex<VertexAttributes...>>(
              PipelineStorageType::STANDART_MODEL,
              render_pass,
              MainPipelineConfigInfo{
                  .vertex_binding_description = Vertex<
                      VertexAttributes...>::getVertexBindingDescription(),
                  .vertex_attribute_descriptions = Vertex<
                      VertexAttributes...>::getVertexAttributeDescription(),
                  .vertex_shader_type =
                      calculateVertexShaderType<Vertex<VertexAttributes...>>(),
                  .fragment_shader_type = ShaderType::DEFAULT_FRAGMENT,
                  .depth_test_enable    = vk::True,
                  .pipeline_layout =
                      manager.getLayout(PipelineLayoutType::STANDART)})),
          m_indices(std::move(indices)),
          m_vertices(std::move(vertices)),
          m_primitive_topology(primitive_topology),
          m_material(material)
    {
    }
    const Pipeline* m_pipeline;

    std::vector<uint32_t> m_indices;

    VertexData m_vertices;

    vk::PrimitiveTopology m_primitive_topology;

    const Material* m_material;
};
} // namespace ars_graphics
