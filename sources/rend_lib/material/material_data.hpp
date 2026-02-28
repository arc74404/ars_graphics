#pragma once

#include "../textures/texture.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace ars_graphics
{

struct PBRParameters
{
    int countTextures() const;

    template <typename T>
    void setAlbedo(const std::vector<T>& arr)
    {
        if (arr.size() >= 4)
        {
            m_albedo_color = glm::vec4(arr[0], arr[1], arr[2], arr[3]);
        }
    }
    template <typename T>
    void setEmissiveFactor(const std::vector<T>& arr)
    {
        if (arr.size() >= 4)
        {
            m_emissive_factor = glm::vec3(arr[0], arr[1], arr[2]);
        }
    }

    glm::vec4 m_albedo_color = glm::vec4(1.0f);

    glm::vec3 m_emissive_factor = glm::vec3(0.0f);

    float m_metallic = 0.0f;

    float m_roughness = 0.5f;

    float m_ao_strength = 1.0;

    const Texture* m_albedo_map{};

    const Texture* m_normal_map{};

    const Texture* m_ao_map{};

    const Texture* m_emissive_map{};

    const Texture* m_metallic_roughness_map{};
};

enum class AlphaMode
{
    MASK,
    BLEND,
    TOTAL_OPAQUE
};

struct MaterialData
{
    PBRParameters m_pbrparams{};
    AlphaMode m_alpha_mode{};
    float m_alpha_cutoff{};
    bool m_double_sided{};
};

} // namespace ars_graphics
