#pragma once

#include <vector>

#include "../shaders/shaders_data_structs/instance_data.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"

namespace ars_graphics
{
struct UboData final
{
    glm::mat4 camera_matrix;
};

struct PerFrameLinksOnData
{
    const std::vector<ModelInstancingData>* m_model_instancing_data;
    const std::vector<MeshInstancingData>* m_mesh_instancing_data;
    const std::vector<InstanceMapping>* m_mapping;
};
} // namespace ars_graphics
