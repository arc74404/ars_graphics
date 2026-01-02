#pragma once
#include <vector>

#include "../material/material.hpp"
#include "../vertex/vertex.hpp"

namespace ars_graphics
{
template <typename... VertexAttributes>
class Mesh
{
public:
    using VertexType = Vertex<VertexAttributes...>;

    Mesh() = default;

    Mesh(std::vector<uint32_t>&& indices, std::vector<VertexType>&& vertices)
        : m_indices(std::move(indices)), m_vertices(std::move(vertices))
    {
    }

    template <typename Buf>
    void push(Buf& buf)
    {
        buf.pushInds(m_indices);
        buf.template pushVerts<VertexType>(m_vertices);
    }

private:
    std::vector<uint32_t> m_indices;

    std::vector<VertexType> m_vertices;
};
} // namespace ars_graphics
