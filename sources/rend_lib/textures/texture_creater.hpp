#pragma once
#include <optional>

#include "texture.hpp"
#include "texture_data.hpp"

namespace ars_graphics
{
class TextureCreater
{
public:
    TextureCreater(const LogicalDevice& logical_device,
                   const PhysicalDevice& physical_device);

    std::optional<Texture> createPBRTexture(const TextureData& data,
                                            Texture::TextureType type) const;

private:
    const LogicalDevice& m_logical_device;
    const PhysicalDevice& m_physical_device;
};
} // namespace ars_graphics
