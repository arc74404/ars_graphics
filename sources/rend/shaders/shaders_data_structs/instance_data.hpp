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
} // namespace ars_graphics
