#pragma once
#include <vector>

#include "../material/material.hpp"
#include "../vertex/vertex.hpp"
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "../material/material.hpp"

namespace ars_graphics
{

class IPrimitive
{
public:
    virtual ~IPrimitive() = default;
};

template <typename... VertexAttributes>
struct Primitive : public IPrimitive
{
    using VertexType = Vertex<VertexAttributes...>;

    Primitive(std::vector<uint32_t>&& indices,
              std::vector<VertexType>&& vertices,
              vk::PrimitiveTopology primitive_topology,
              const Material* material)
        : m_indices(std::move(indices)),
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
