#pragma once

#include "glm/mat4x4.hpp"

namespace ars_graphics
{
struct ModelInstancingData
{
    glm::mat4 transform_matrix;
};

struct MeshInstancingData
{
    glm::mat4 transform_matrix;
};

struct InstanceMapping
{
    uint32_t model_instance_idx;
    uint32_t mesh_instance_idx;
};

} // namespace ars_graphics
