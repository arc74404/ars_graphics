#include "mesh.hpp"

namespace ars_graphics
{
void
Mesh::addPrimitive(Primitive&& primitive)
{
    m_primitives.emplace_back(std::move(primitive));
}

const std::vector<Primitive>&
Mesh::getPrimitives() const
{
    return m_primitives;
}
} // namespace ars_graphics
