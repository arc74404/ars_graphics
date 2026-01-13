#pragma once
#include <vector>

#include "../util_funcs.hpp"

namespace ars_graphics
{

struct VertexData
{
    template <typename VertexType>
    VertexData(const std::vector<VertexType>& vertices)
    {
        m_data.reserve(vertices.size() * VertexType::getStride());

        for (auto&& vertex : vertices)
        {
            vertex.pushBytes(m_data);
        }
        m_vertex_offset = vertices.size();
    }

    uint32_t vertexOffset() const
    {
        return m_vertex_offset;
    }

    template <bool need_alloc>
    void pushData(std::vector<char>& dest) const
    {
        pushDataToTheEnd<need_alloc>(dest, m_data);
    }

private:
    std::vector<char> m_data;

    uint32_t m_vertex_offset;
};
} // namespace ars_graphics
