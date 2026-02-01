#pragma once

#include <vector>

#include "../model/model.hpp"
#include "../render/render_info.hpp"
#include "../shaders/shaders_data_structs/instance_data.hpp"

#include "instancer.hpp"

namespace ars_graphics
{
class MeshNodeInstancing
{
public:
    void initData(const std::vector<ModelNode>& nodes, uint32_t meshes_count);

    bool noInstanceForThisMesh(size_t index) const
    {
        return m_meshes_data[index].m_data.empty();
    }

    void fillInstancer(
        Instancer<MeshInstancingData, StorageBuffer>& model_inst_data,
        size_t mesh_index) const
    {
        for (auto&& m : m_meshes_data[mesh_index].m_data)
        {
            model_inst_data.push(m);
        }
    }

    size_t countInstances(size_t mesh_index) const
    {
        return m_meshes_data[mesh_index].m_data.size();
    }

private:
    std::vector<PerMeshInstancingData> m_meshes_data;
};
} // namespace ars_graphics
