#pragma once
#include <vector>

#include "../material/material.hpp"
#include "../pipelines/pipeline.hpp"
#include "../pipelines/pipeline_layout_storage.hpp"
#include "../pipelines/pipeline_manager.hpp"
#include "../vertex/vertex.hpp"

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
                  .vertex_shader_type   = ShaderType::DEFAULT_3D_VERTEX,
                  .fragment_shader_type = ShaderType::DEFAULT_2D_VERTEX,
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

class Mesh final
{
public:
    Mesh() = default;

    template <typename... VertexAttributes>
    void addPrimitive(Primitive<VertexAttributes...>&& primitive)
    {
        m_primitives.emplace_back(
            std::make_unique<IPrimitive>(std::move(primitive)));
    }

    // template <typename Buf>
    // void push(Buf& buf)
    // {
    //     buf.pushInds(m_primitives.m_indices);
    //     buf.template pushVerts<VertexType>(m_primitives.m_vertices);
    // }

private:
    std::vector<std::unique_ptr<IPrimitive>> m_primitives;
};
} // namespace ars_graphics
