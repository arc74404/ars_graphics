#include "model.hpp"

namespace ars_graphics
{
void
Model::addMesh(Mesh&& mesh)
{
    m_meshes.emplace_back(std::move(mesh));
}

const std::vector<Mesh>&
Model::getMeshes() const
{
    return m_meshes;
}
} // namespace ars_graphics
