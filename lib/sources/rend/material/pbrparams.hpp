#pragma once

#include "../textures/texture.hpp"
#include "glm/vec4.hpp"

namespace ars_graphics
{
struct PBRParameters
{
    inline int countTextures() const
    {
        return (albedo_map != nullptr) + (normal_map != nullptr) +
               (ao_map != nullptr) + (emissive_map != nullptr) +
               (height_map != nullptr) + (metallic_roughness_map != nullptr);
    }

    glm::vec4 albedo_color = glm::vec4(1.0f);

    float metallic = 0.0f;

    float roughness = 0.5f;

    float ao = 1.0;

    const Texture* albedo_map = nullptr;

    const Texture* normal_map = nullptr;

    const Texture* ao_map = nullptr;

    const Texture* emissive_map = nullptr;

    const Texture* height_map = nullptr;

    const Texture* metallic_roughness_map = nullptr;
};
} // namespace ars_graphics
