#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace ars_graphics::attributes
{
using Position2D = glm::vec2;
using Position3D = glm::vec3;

using Normal = glm::vec3;

using TextureCoord = glm::vec2;

using ColorRGB  = glm::vec3;
using ColorRGBA = glm::vec4;

using Tangent   = glm::vec3;
using Bitangent = glm::vec3;
} // namespace ars_graphics::attributes
