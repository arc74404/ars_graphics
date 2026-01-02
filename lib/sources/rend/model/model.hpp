#pragma once
#include <vector>

namespace ars_graphics
{
template <typename MeshType>
class Model
{
public:
    void addMesh(MeshType&& mesh);

private:
    std::vector<MeshType> m_meshes;
};
} // namespace ars_graphics
