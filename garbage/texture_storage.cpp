#include "texture_storage.hpp"

#include <array>

namespace ars_graphics
{

TextureStorage::TextureStorage(const LogicalDevice& logical_device,
                               const PhysicalDevice& physical_device)
{
    auto makeDummy = [&logical_device,
                      &physical_device](std::unique_ptr<Texture>& dst,
                                        std::array<unsigned char, 4> temp_data)
    {
        TextureData white;
        white.setData(temp_data.data(), 4, 1, 1, 4);
        dst = std::make_unique<Texture>(logical_device, physical_device, white,
                                        TextureConfigInfo{});
    };

    makeDummy(m_dummy_white, {255, 255, 255, 255});
    makeDummy(m_dummy_black, {0, 0, 0, 255});
    makeDummy(m_dummy_normal, {128, 128, 255, 255});
    makeDummy(m_dummy_default_albedo, {180, 180, 180, 255});
    makeDummy(m_dummy_default_mr, {0, 255, 0, 255});
}

const Texture*
TextureStorage::getDummy(Texture::TextureType type)
{
    switch (type)
    {
        case Texture::TextureType::ALBEDO:
            return m_dummy_default_albedo.get();

        case Texture::TextureType::EMISSIVE:
            return m_dummy_black.get();

        case Texture::TextureType::METALLIC_ROUGHNESS:
            return m_dummy_white.get();

        case Texture::TextureType::NORMAL:
            return m_dummy_normal.get();

        case Texture::TextureType::OCCLUSION:
            return m_dummy_white.get();

        default:
            return m_dummy_white.get();
    }
}

} // namespace ars_graphics
