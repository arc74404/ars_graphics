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
              calculateVertexShaderType<Vertex<VertexAttributes...>>())
              m_indices(std::move(indices)),
          m_vertex_binding_description(),
          m_vertex_attribute_description(),
          m_vertices(std::move(vertices)),
          m_primitive_topology(primitive_topology),
          m_material(material)
    {
    }

    ShaderType m_vertex_shader_type;

    const vk::VertexInputBindingDescription& m_vertex_binding_description;

    const std::vector<vk::VertexInputAttributeDescription>&
        m_vertex_attribute_description;

    std::vector<uint32_t> m_indices;

    VertexData m_vertices;

    vk::PrimitiveTopology m_primitive_topology;

    const Material* m_material;
};
} // namespace ars_graphics
