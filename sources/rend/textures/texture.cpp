#include "texture.hpp"

#include "../buffers/staging_buffer.hpp"
#include "../cmdbuf/command_buffer.hpp"

#include "stb_image.h"

namespace ars_graphics
{

bool
TextureSetCompare::operator()(const Texture& a, const Texture& b) const
{
    return &a < &b;
}

void
Texture::initImage(const LogicalDevice& logical_device,
                   const PhysicalDevice& physical_device,
                   const TextureData& data,
                   const TextureConfigInfo& config_info)
{
    
    if (data.getChannels() != 4)
    {
    }

    vk::DeviceSize imageSize = data.dataSize();

    StagingBuffer staging_buffer;
    staging_buffer.setData(logical_device, physical_device, data.data(),
                           imageSize);

    m_image =
        Image(logical_device, physical_device,
              ImageConfigInfo{.format = vk::Format::eR8G8B8A8Unorm,
                              .width  = static_cast<uint32_t>(data.getWidth()),
                              .height = static_cast<uint32_t>(data.getHeight()),
                              .usage_flags =
                                  vk::ImageUsageFlagBits::eSampled |
                                  vk::ImageUsageFlagBits::eTransferDst |
                                  vk::ImageUsageFlagBits::eColorAttachment});

    CommandBuffer command_buffer{logical_device};

    auto queue = logical_device.getQueue("graphics");

    Buffer::copyBufferToImage(logical_device, physical_device, &staging_buffer,
                              m_image, queue, command_buffer.get(),
                              data.getWidth(), data.getHeight());

    vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &(command_buffer.get()));

    queue.submit(1, &submitInfo, nullptr);
    queue.waitIdle();
}

void
Texture::initSampler(const LogicalDevice& logical_device,
                     const PhysicalDevice& physical_device,
                     const TextureConfigInfo& config_info)
{
    vk::SamplerCreateInfo sampler_info{};

    sampler_info.magFilter = config_info.m_mag_filter;
    sampler_info.minFilter = config_info.m_min_filter;

    sampler_info.addressModeU = config_info.m_address_mode_u;
    sampler_info.addressModeV = config_info.m_address_mode_v;
    sampler_info.addressModeW = config_info.m_address_mode_w;

    sampler_info.anisotropyEnable = config_info.m_enable_anisotropy;
    sampler_info.maxAnisotropy =
        config_info.m_enable_anisotropy
            ? physical_device.get().getProperties().limits.maxSamplerAnisotropy
            : 1.0f;

    sampler_info.borderColor = vk::BorderColor::eIntOpaqueBlack;

    sampler_info.unnormalizedCoordinates = false;

    sampler_info.compareEnable = false;
    sampler_info.compareOp     = vk::CompareOp::eAlways;

    sampler_info.mipmapMode = vk::SamplerMipmapMode::eLinear;
    sampler_info.mipLodBias = 0.0f;
    sampler_info.minLod     = 0.0f;
    sampler_info.maxLod     = VK_LOD_CLAMP_NONE;

    auto&& res = logical_device.get().createSamplerUnique(sampler_info);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create sampler");
    }

    m_sampler = std::move(res.value);
}

Texture::Texture(const LogicalDevice& logical_device,
                 const PhysicalDevice& physical_device,
                 const TextureData& data,
                 const TextureConfigInfo& config_info)
{
    initImage(logical_device, physical_device, data, config_info);

    initSampler(logical_device, physical_device, config_info);
}

const vk::ImageView&
Texture::getImageView() const
{
    return m_image.view();
}

const vk::Sampler&
Texture::getSampler() const
{
    return m_sampler.get();
}

// void
// Texture::loadTexture(const LogicalDevice& logical_device,
//                      const PhysicalDevice& physical_device,
//                      std::string_view file_path)
// {
//     int tex_channels;

//     int width, height;

//     stbi_uc* pixels = stbi_load(file_path.data(), &width, &height,
//                                 &tex_channels, STBI_rgb_alpha);
//     if (!pixels)
//     {
//         throw std::runtime_error(
//             std::format("{}{}", "Failed to load texture file: ",
//             file_path));
//     }
//     vk::DeviceSize imageSize = width * height * 4;

//     StagingBuffer staging_buffer;
//     staging_buffer.setData(logical_device, physical_device, pixels,
//     imageSize); stbi_image_free(pixels);

//     CommandBuffer command_buffer{logical_device};
//     auto queue = logical_device.getQueue("graphics");

//     m_image = Image(logical_device, physical_device,
//                     {.format      = vk::Format::eR8G8B8A8Unorm,
//                      .width       = static_cast<uint32_t>(width),
//                      .height      = static_cast<uint32_t>(height),
//                      .usage_flags = vk::ImageUsageFlagBits::eSampled |
//                                     vk::ImageUsageFlagBits::eTransferDst
//                                     |
//                                     vk::ImageUsageFlagBits::eColorAttachment,
//                      .aspect_mask = vk::ImageAspectFlagBits::eColor});

//     Buffer::copyBufferToImage(logical_device, physical_device,
//     &staging_buffer,
//                               m_image, queue, command_buffer.get(),
//                               width, height);

//     // Отправка команд
//     vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1,
//     &(command_buffer.get()));

//     queue.submit(1, &submitInfo, nullptr);
//     queue.waitIdle();

//     // Sampler
//     vk::SamplerCreateInfo samplerInfo({},                              //
//                                       vk::Filter::eLinear,             //
//                                       vk::Filter::eLinear,             //
//                                       vk::SamplerMipmapMode::eLinear,  //
//                                       vk::SamplerAddressMode::eRepeat, //
//                                       vk::SamplerAddressMode::eRepeat, //
//                                       vk::SamplerAddressMode::eRepeat, //
//                                       0.0f,                            //
//                                       false,                           //
//                                       16.0f,                           //
//                                       false,                           //
//                                       vk::CompareOp::eNever,
//                                       0.0f, //
//                                       std::numeric_limits<float>::max(),
//                                       vk::BorderColor::eFloatOpaqueBlack);

//     auto&& res = logical_device.get().createSamplerUnique(samplerInfo);
//     if (res.result != vk::Result::eSuccess)
//     {
//         throw std::runtime_error("Failed create sampler");
//     }
//     m_sampler = std::move(res.value);
// }

// void
// Texture::allocDescriptorSet(const LogicalDevice& logical_device,
//                             const DescriptorManager& desc_manager)
// {

//     desc_manager.getAllocator(DescriptorSetLayoutType::COMBINED_IMAGE_SAMPLER)
//         .allocate(logical_device, m_descriptor_set);

//     vk::DescriptorImageInfo imageInfo(m_sampler.get(), m_image.view(),
//                                       vk::ImageLayout::eShaderReadOnlyOptimal);
//     vk::WriteDescriptorSet descriptorWrite(
//         m_descriptor_set.get(), 0, 0, 1,
//         vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr,
//         nullptr);

//     logical_device.get().updateDescriptorSets(1, &descriptorWrite, 0,
//     nullptr);
// }

} // namespace ars_graphics
