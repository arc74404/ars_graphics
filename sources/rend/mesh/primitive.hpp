#pragma once

#include "../material/material.hpp"
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
              const NonUpdatebleMaterial* material)
        : m_indices(std::move(indices)),
          m_vertices(std::move(vertices)),
          m_vertex_binding_description(
              Vertex<VertexAttributes...>::getVertexBindingDescription()),
          m_vertex_attribute_description(
              Vertex<VertexAttributes...>::getVertexAttributeDescription()),
          m_material(material)
    {
    }

    const vk::VertexInputBindingDescription& m_vertex_binding_description;
    const std::vector<vk::VertexInputAttributeDescription>&
        m_vertex_attribute_description;

    std::vector<uint32_t> m_indices;

    VertexData m_vertices;

    const NonUpdatebleMaterial* m_material;
};
} // namespace ars_graphics
