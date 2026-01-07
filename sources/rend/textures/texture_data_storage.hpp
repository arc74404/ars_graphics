#pragma once
#include <vector>

#include "glm/vec2.hpp"

#include "texture_data.hpp"

namespace ars_graphics
{

class TextureDataStorage
{
public:
    std::vector<TextureData>& pushTextures(
        std::vector<TextureData>&& new_textures_data);

private:
    std::vector<std::vector<TextureData>> m_texture_data_storage;
};

}; // namespace ars_graphics
