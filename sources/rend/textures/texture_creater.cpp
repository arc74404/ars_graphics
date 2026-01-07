#include "texture_creater.hpp"

#include <iostream>
namespace ars_graphics
{
TextureCreater::TextureCreater(const LogicalDevice& logical_device,
                               const PhysicalDevice& physical_device)
    : m_logical_device(logical_device), m_physical_device(physical_device)
{
}

std::optional<Texture>
TextureCreater::createPBRTexture(const TextureData* data,
                                 Texture::TextureType type) const
{
    TextureConfigInfo config_info;
    switch (type)
    {
        case Texture::TextureType::ALBEDO:
        {
            config_info.m_srgb              = true;
            config_info.m_address_mode_u    = vk::SamplerAddressMode::eRepeat;
            config_info.m_address_mode_v    = vk::SamplerAddressMode::eRepeat;
            config_info.m_mag_filter        = vk::Filter::eLinear;
            config_info.m_min_filter        = vk::Filter::eLinear;
            config_info.m_mipmap_mode       = vk::SamplerMipmapMode::eLinear;
            config_info.m_enable_anisotropy = true;
            break;
        }

        case Texture::TextureType::METALLIC_ROUGHNESS:
        {
            config_info.m_srgb              = false;
            config_info.m_address_mode_u    = vk::SamplerAddressMode::eRepeat;
            config_info.m_address_mode_v    = vk::SamplerAddressMode::eRepeat;
            config_info.m_mag_filter        = vk::Filter::eLinear;
            config_info.m_min_filter        = vk::Filter::eLinear;
            config_info.m_mipmap_mode       = vk::SamplerMipmapMode::eLinear;
            config_info.m_enable_anisotropy = true;
            config_info.m_enable_anisotropy = false;
            break;
        }

        case Texture::TextureType::NORMAL:
        {
            config_info.m_srgb              = false;
            config_info.m_address_mode_u    = vk::SamplerAddressMode::eRepeat;
            config_info.m_address_mode_v    = vk::SamplerAddressMode::eRepeat;
            config_info.m_mag_filter        = vk::Filter::eLinear;
            config_info.m_min_filter        = vk::Filter::eLinear;
            config_info.m_mipmap_mode       = vk::SamplerMipmapMode::eLinear;
            config_info.m_enable_anisotropy = false;
            break;
        }

        case Texture::TextureType::OCCLUSION:
        {
            config_info.m_srgb              = false;
            config_info.m_address_mode_u    = vk::SamplerAddressMode::eRepeat;
            config_info.m_address_mode_v    = vk::SamplerAddressMode::eRepeat;
            config_info.m_mag_filter        = vk::Filter::eLinear;
            config_info.m_min_filter        = vk::Filter::eLinear;
            config_info.m_mipmap_mode       = vk::SamplerMipmapMode::eLinear;
            config_info.m_enable_anisotropy = false;
            config_info.m_generate_mipmaps  = false;
            break;
        }

        case Texture::TextureType::EMISSIVE:
        {
            config_info.m_srgb              = true;
            config_info.m_address_mode_u    = vk::SamplerAddressMode::eRepeat;
            config_info.m_address_mode_v    = vk::SamplerAddressMode::eRepeat;
            config_info.m_mag_filter        = vk::Filter::eLinear;
            config_info.m_min_filter        = vk::Filter::eLinear;
            config_info.m_mipmap_mode       = vk::SamplerMipmapMode::eLinear;
            config_info.m_enable_anisotropy = true;
            break;
        }

        default:
            return std::nullopt;
    }
    try
    {
        Texture res(m_logical_device, m_physical_device, data, config_info);
        return res;
    }
    catch (std::runtime_error& error)
    {
        std::cout << error.what() << '\n';
    }
    return std::nullopt;
}
} // namespace ars_graphics
