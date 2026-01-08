#pragma once

#include <vector>

#include "glm/vec2.hpp"

#include "texture.hpp"

namespace ars_graphics
{

class TextureStorage
{
public:
    template <typename Cont>
    void pushTextures(Cont&& cont)
    {
        auto& dest = m_texture_storage.emplace_back();
        dest.reserve(cont.size());

        for (Texture& el : cont)
        {
            dest.emplace_back(std::move(el));
        }
    }

private:
    std::vector<std::vector<Texture>> m_texture_storage;
};

}; // namespace ars_graphics
