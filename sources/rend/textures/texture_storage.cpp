#include "texture_storage.hpp"

namespace ars_graphics
{
std::vector<Texture>&
TextureStorage::pushTextures(std::vector<Texture>&& new_textures)
{
    return m_texture_storage.emplace_back(std::move(new_textures));
}
} // namespace ars_graphics
