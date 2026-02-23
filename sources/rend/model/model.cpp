#include "model.hpp"

#include <glm/gtc/matrix_transform.hpp>
namespace ars_graphics
{
// void
// Model::addMesh(Mesh&& mesh)
// {
//     m_nodes.clear();
//     m_meshes.emplace_back(std::move(mesh));
// }

// void
// Model::addNode(ParseModelNodeData&& node_data)
// {
//     ModelNode temp;
//     temp.m_mesh_index      = node_data.mesh_index;
//     temp.m_local_transform = std::move(node_data.matrix);

//     m_nodes.emplace_back(std::move(temp));
// }

// void
// Model::connectGraph(std::vector<std::vector<int>>&& node_childrens)
// {
//     for (uint32_t node_index = 0; node_index < node_childrens.size();
//          ++node_index)
//     {
//         m_nodes[node_index].m_children_indices =
//             std::move(node_childrens[node_index]);
//     }
// }

Model::Model(const std::vector<Mesh>& meshes,
             const std::vector<ModelNode>& model_graph)
    : m_meshes(meshes), m_graph(model_graph)
{
}

Model::Model(std::vector<Mesh>&& meshes, std::vector<ModelNode>&& model_graph)
    : m_graph(std::move(model_graph)), m_meshes(std::move(meshes))
{
}

const std::vector<Mesh>&
Model::getMeshes() const noexcept
{
    return m_meshes;
}

const std::vector<ModelNode>&
Model::getGraph() const noexcept
{
    return m_graph;
}

} // namespace ars_graphics
