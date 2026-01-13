#pragma once
#include <memory>
#include <tuple>
#include <vector>

#include "../mesh/mesh.hpp"

namespace ars_graphics
{
class Model final
{
public:
    void addMesh(Mesh&& mesh);

    const std::vector<Mesh>& getMeshes() const;

private:
    std::vector<Mesh> m_meshes;
};
} // namespace ars_graphics
