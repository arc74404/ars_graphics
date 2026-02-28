#pragma once
#include <memory>
#include <tuple>
#include <vector>

#include "../mesh/mesh.hpp"
#include "glm/mat4x4.hpp"

namespace ars_graphics
{

struct ModelNode final
{
    int m_mesh_index = -1;

    glm::mat4 m_local_transform = glm::mat4(1.0f);

    std::vector<int> m_children_indices;
};

class Model final
{
public:
    Model(const std::vector<Mesh>& meshes,
          const std::vector<ModelNode>& model_graph);

    Model(std::vector<Mesh>&& meshes, std::vector<ModelNode>&& model_graph);

    const std::vector<ModelNode>& getGraph() const noexcept;

    const std::vector<Mesh>& getMeshes() const noexcept;

private:
    std::vector<ModelNode> m_graph;

    std::vector<Mesh> m_meshes;
};
} // namespace ars_graphics
