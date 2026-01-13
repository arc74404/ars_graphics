#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <string_view>

#include "../descriptors/descriptor_manager.hpp"
#include "../device/logical_device.hpp"

#include "image.hpp"
#include "texture_data.hpp"

namespace ars_graphics
{

struct TextureConfigInfo
{
    vk::Filter m_mag_filter                 = vk::Filter::eLinear;
    vk::Filter m_min_filter                 = vk::Filter::eLinear;
    vk::SamplerAddressMode m_address_mode_u = vk::SamplerAddressMode::eRepeat;
    vk::SamplerAddressMode m_address_mode_v = vk::SamplerAddressMode::eRepeat;
    vk::SamplerAddressMode m_address_mode_w = vk::SamplerAddressMode::eRepeat;

    bool m_enable_anisotropy = true;
    bool m_generate_mipmaps  = true;
    bool m_srgb              = false;

    float m_min_lod      = 0.0f;
    float m_max_lod      = 1000.0f;
    float m_mip_lod_bias = 0.0f;

    vk::SamplerMipmapMode m_mipmap_mode = vk::SamplerMipmapMode::eLinear;

    vk::CompareOp m_compare_op = vk::CompareOp::eAlways;
    bool m_compare_enable      = false;

    vk::BorderColor m_border_color = vk::BorderColor::eFloatOpaqueBlack;

    bool m_unnormalized_coordinates = false;
};

class Texture
{
public:
    enum class TextureType
    {
        ALBEDO,
        METALLIC_ROUGHNESS,
        NORMAL,
        OCCLUSION,
        EMISSIVE
    };

    Texture() = default;

    Texture(const LogicalDevice& logical_device,
            const PhysicalDevice& physical_device,
            const TextureData& data,
            const TextureConfigInfo& config_info);

    const vk::ImageView& getImageView() const;

    const vk::Sampler& getSampler() const;

private:
    void initImage(const LogicalDevice& logical_device,
                   const PhysicalDevice& physical_device,
                   const TextureData& data,
                   const TextureConfigInfo& config_info);

    void initSampler(const LogicalDevice& logical_device,
                     const PhysicalDevice& physical_device,
                     const TextureConfigInfo& config_info);
    Image m_image;
    vk::UniqueSampler m_sampler;
};

struct TextureSetCompare
{
    bool operator()(const Texture& a, const Texture& b) const;
};

} // namespace ars_graphics
