#pragma once

#include <vector>

#include "glm/vec2.hpp"

#include "texture.hpp"

namespace ars_graphics
{

class TextureStorage
{
public:
    std::vector<Texture>& pushTextures(std::vector<Texture>&& new_textures);

private:
    std::vector<std::vector<Texture>> m_texture_storage;
};

}; // namespace ars_graphics
