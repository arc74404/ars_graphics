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

    // bool isRoot() const
    // {
    //     return (m_parent_index == -1);
    // }

    int m_mesh_index = -1;

    glm::mat4 m_local_transform = glm::mat4(1.0f);

    // int m_parent_index = -1;

    std::vector<int> m_children_indices;
};

struct ParseModelNodeData final
{
    int mesh_index;
    glm::mat4 matrix;
};

class Model final
{
public:
    void addMesh(Mesh&& mesh);

    void addNode(ParseModelNodeData&& node_data);

    void connectGraph(std::vector<std::vector<int>>&& node_childrens);

    void setRoots(std::vector<int>&& roots);

    const std::vector<int>& getRoots() const;

    const std::vector<ModelNode>& getNodes() const;

    const std::vector<Mesh>& getMeshes() const;

private:
    std::vector<int> m_roots;

    std::vector<ModelNode> m_nodes;

    std::vector<Mesh> m_meshes;
};
} // namespace ars_graphics
