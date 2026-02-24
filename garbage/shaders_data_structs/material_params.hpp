#pragma once

#include <glm/vec4.hpp>

namespace ars_graphics::shaders_params
{
struct MaterialParams
{
    glm::vec4 albedo_factor;

    glm::vec4 emissive_factor;

    float metallic;

    float roughness;

    float ao_strength;
};
} // namespace ars_graphics::shaders_params
