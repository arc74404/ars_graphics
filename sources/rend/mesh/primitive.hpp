#pragma once

#include "../shaders/calculate_vertex_shader_type.hpp"
#include "../vertex/vertex.hpp"
#include "../vertex/vertex_data.hpp"

namespace ars_graphics
{
struct Primitive final
{
    template <typename... VertexAttributes>
    Primitive(std::vector<uint32_t>&& indices,
              const std::vector<Vertex<VertexAttributes...>>& vertices,
              vk::PrimitiveTopology primitive_topology,
              const Material* material)
        : m_vertex_shader_type(
              calculateVertexShaderType<Vertex<VertexAttributes...>>()),
          m_indices(std::move(indices)),
          m_vertex_binding_description(
              Vertex<VertexAttributes...>::getVertexBindingDescription()),
          m_vertex_attribute_description(
              Vertex<VertexAttributes...>::getVertexAttributeDescription()),
          m_vertices(std::move(vertices)),
          m_primitive_topology(primitive_topology),
          m_material(material),
          m_key_vertex(Vertex<VertexAttributes...>::getIntRepersentation())
    {
    }

    ShaderType m_vertex_shader_type;
    const vk::VertexInputBindingDescription& m_vertex_binding_description;
    const std::vector<vk::VertexInputAttributeDescription>&
        m_vertex_attribute_description;

    uint64_t m_key_vertex;

    std::vector<uint32_t> m_indices;

    VertexData m_vertices;

    vk::PrimitiveTopology m_primitive_topology;

    const Material* m_material;
};
} // namespace ars_graphics
