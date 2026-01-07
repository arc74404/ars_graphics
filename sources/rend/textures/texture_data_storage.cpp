#include "texture_data_storage.hpp"

#include <fstream>
#include <iostream>

namespace ars_graphics
{
std::vector<TextureData>&
TextureDataStorage::pushTextures(std::vector<TextureData>&& new_textures_data)
{
    return m_texture_data_storage.emplace_back(std::move(new_textures_data));
}

}; // namespace ars_graphics
