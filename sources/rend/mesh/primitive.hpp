#pragma once

#include "../pipelines/pipeline.hpp"
#include "../pipelines/pipeline_manager.hpp"

namespace ars_graphics
{

class IPrimitive
{
public:
    virtual ~IPrimitive() = default;

    IPrimitive(const Pipeline* pipeline) : m_pipeline(pipeline)
    {
    }

private:
    const Pipeline* m_pipeline;
};

template <typename... VertexAttributes>
struct Primitive : public IPrimitive
{
    using VertexType = Vertex<VertexAttributes...>;

    Primitive(PipelineManager& manager,
              const vk::RenderPass& render_pass,
              std::vector<uint32_t>&& indices,
              std::vector<VertexType>&& vertices,
              vk::PrimitiveTopology primitive_topology,
              const Material* material)
        : IPrimitive(manager.getPipeline<VertexType>(
              PipelineStorageType::STANDART_MODEL,
              render_pass,
              MainPipelineConfigInfo{
                  .vertex_binding_description =
                      VertexType::getVertexBindingDescription(),
                  .vertex_attribute_descriptions =
                      VertexType::getVertexAttributeDescription(),
                  .vertex_shader_type = calculateVertexShaderType<
                      Primitive<VertexAttributes...>>(),
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

    std::vector<uint32_t> m_indices;

    std::vector<VertexType> m_vertices;

    vk::PrimitiveTopology m_primitive_topology;

    const Material* m_material;
};
} // namespace ars_graphics
