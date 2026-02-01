#include "model.hpp"

namespace ars_graphics
{
void
Model::addMesh(Mesh&& mesh)
{
    m_nodes.clear();
    m_meshes.emplace_back(std::move(mesh));
}

void
Model::addNode(ParseModelNodeData&& node_data)
{
    ModelNode temp;
    temp.m_mesh_index - node_data.mesh_index;
    temp.m_local_transform = std::move(node_data.matrix);
    m_nodes.emplace_back(std::move(temp));
}

void
Model::connectGraph(std::vector<std::vector<int>>&& node_childrens)
{
    for (uint32_t node_index = 0; node_index < node_childrens.size();
         ++node_index)
    {
        m_nodes[node_index].m_children_indices =
            std::move(node_childrens[node_index]);

        for (uint32_t j = 0; j < node_childrens[node_index].size(); ++j)
        {
            uint32_t child_index = node_childrens[node_index][j];

            m_nodes[child_index].m_parent_index = node_index;
        }
    }
}

const std::vector<Mesh>&
Model::getMeshes() const
{
    return m_meshes;
}

const std::vector<ModelNode>&
Model::getNodes() const
{
    return m_nodes;
}

} // namespace ars_graphics
