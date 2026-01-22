#pragma once

#include <memory>
#include <vector>

#include "../device/logical_device.hpp"
#include "../device/physical_device.hpp"
#include "glm/vec2.hpp"

#include "texture.hpp"

namespace ars_graphics
{

class TextureStorage
{
public:
    TextureStorage(const LogicalDevice& logical_device,
                   const PhysicalDevice& physical_device);

    template <typename Cont>
    std::vector<Texture>& pushTextures(Cont&& cont)
    {
        auto& dest = m_texture_storage.emplace_back();
        dest.reserve(cont.size());

        for (Texture& el : cont)
        {
            dest.emplace_back(std::move(el));
        }
        return dest;
    }

    const Texture* getDummy(Texture::TextureType type);

private:
    std::vector<std::vector<Texture>> m_texture_storage;

    std::unique_ptr<Texture> m_dummy_white;
    std::unique_ptr<Texture> m_dummy_black;
    std::unique_ptr<Texture> m_dummy_normal;
    std::unique_ptr<Texture> m_dummy_default_albedo;
    std::unique_ptr<Texture> m_dummy_default_mr;
};

}; // namespace ars_graphics
