#pragma once
#include <vector>

namespace ars_graphics
{
template <typename MeshType>
class Model
{
public:
    void addMesh(MeshType&& mesh)
    {
        m_meshes.emplace_back(std::move(mesh));
    }

private:
    std::vector<MeshType> m_meshes;
};
} // namespace ars_graphics
