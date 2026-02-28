#include "mesh_node_instancing.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace ars_graphics
{
void
fillByChildren(std::vector<PerMeshInstancingData>& res,
               const std::vector<ModelNode>& nodes,
               const glm::mat4& parent_matrix,
               size_t node_index,
               std::vector<bool>& has_marked)
{
    has_marked[node_index] = true;

    for (auto&& child_index : nodes[node_index].m_children_indices)
    {
        if (has_marked[child_index])
        {
            continue;
        }
        glm::mat4 world_transform =
            parent_matrix * nodes[child_index].m_local_transform;

        if (nodes[child_index].m_mesh_index != -1)
        {
            res[nodes[child_index].m_mesh_index].m_data.emplace_back(
                world_transform);
        };

        fillByChildren(res, nodes, world_transform, child_index, has_marked);
    }
}

namespace
{
std::vector<PerMeshInstancingData>
takeCore(const std::vector<int>& roots,
         const std::vector<ModelNode>& nodes,
         size_t count_meshes)
{
    std::vector<PerMeshInstancingData> res(count_meshes);

    std::vector<bool> has_marked(nodes.size(), 0);

    for (auto root : roots)
    {
        if (nodes[root].m_mesh_index != -1)
        {
            res[nodes[root].m_mesh_index].m_data.emplace_back(
                nodes[root].m_local_transform);
        };
        fillByChildren(res, nodes, nodes[root].m_local_transform, root,
                       has_marked);
    }
    return res;
}

} // namespace

void
MeshNodeInstancing::initData(const std::vector<int>& roots,
                             const std::vector<ModelNode>& nodes,
                             uint32_t meshes_count)
{
    m_meshes_data = takeCore(roots, nodes, meshes_count);
}
} // namespace ars_graphics
