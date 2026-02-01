#include "mesh_node_instancing.hpp"

namespace ars_graphics
{
void
fillByChildren(std::vector<PerMeshInstancingData>& res,
               const std::vector<ModelNode>& nodes,
               const glm::mat4& parent_matrix,
               const ModelNode& node)
{
    glm::mat4 world_transform = parent_matrix * node.m_local_transform;

    if (node.m_mesh_index != -1)
    {
        res[node.m_mesh_index].m_data.emplace_back(world_transform);
    };

    for (auto&& child_index : node.m_children_indices)
    {
        fillByChildren(res, nodes, world_transform, nodes[child_index]);
    }
}

namespace
{
std::vector<PerMeshInstancingData>
sortNodesByMeshIndexAndTakeCore(const std::vector<ModelNode>& nodes,
                                size_t count_meshes)
{
    std::vector<PerMeshInstancingData> res(count_meshes);

    for (auto&& node : nodes)
    {
        if (false == node.isRoot())
        {
            continue;
        }
        if (node.m_mesh_index != -1)
        {
            res[node.m_mesh_index].m_data.emplace_back(node.m_local_transform);
        }
        else
        {
            fillByChildren(res, nodes, node.m_local_transform, node);
        }
    }

    return res;
}

} // namespace

void
MeshNodeInstancing::initData(const std::vector<ModelNode>& nodes,
                             uint32_t meshes_count)
{
    m_meshes_data = sortNodesByMeshIndexAndTakeCore(nodes, meshes_count);
}
} // namespace ars_graphics
