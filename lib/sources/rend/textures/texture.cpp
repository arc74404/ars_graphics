#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "../buffers/staging_buffer.hpp"
#include "../cmdbuf/command_buffer.hpp"

#include "stb_image.h"

namespace ars_graphics
{
Texture::Texture(const LogicalDevice& logical_device,
                 const PhysicalDevice& physical_device)
    : m_image(logical_device,
              physical_device,
              {.format      = vk::Format::eR8G8B8A8Unorm,
               .width       = static_cast<uint32_t>(m_width),
               .height      = static_cast<uint32_t>(m_height),
               .usage_flags = vk::ImageUsageFlagBits::eSampled |
                              vk::ImageUsageFlagBits::eTransferDst |
                              vk::ImageUsageFlagBits::eColorAttachment,
               .aspect_mask = vk::ImageAspectFlagBits::eColor})
{
}

int
Texture::getWidth() const noexcept
{
    return m_width;
}

int
Texture::getHeight() const noexcept
{
    return m_height;
}

void
Texture::loadTexture(const LogicalDevice& logical_device,
                     const PhysicalDevice& physical_device,
                     std::string_view file_path)
{
    int tex_channels;

    stbi_uc* pixels = stbi_load(file_path.data(), &m_width, &m_height,
                                &tex_channels, STBI_rgb_alpha);
    if (!pixels)
    {
        throw std::runtime_error(
            std::format("{}{}", "Failed to load texture file: ", file_path));
    }
    vk::DeviceSize imageSize = m_width * m_height * 4;

    StagingBuffer staging_buffer;
    staging_buffer.setData(logical_device, physical_device, pixels, imageSize);
    stbi_image_free(pixels);

    CommandBuffer command_buffer{logical_device};
    auto queue = logical_device.getQueue("graphics");

    Buffer::copyBufferToImage(logical_device, physical_device, &staging_buffer,
                              m_image, queue, command_buffer.get(), m_width,
                              m_height);

    // Отправка команд
    vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &(command_buffer.get()));

    queue.submit(1, &submitInfo, nullptr);
    queue.waitIdle();

    // Sampler
    vk::SamplerCreateInfo samplerInfo({},                              //
                                      vk::Filter::eLinear,             //
                                      vk::Filter::eLinear,             //
                                      vk::SamplerMipmapMode::eLinear,  //
                                      vk::SamplerAddressMode::eRepeat, //
                                      vk::SamplerAddressMode::eRepeat, //
                                      vk::SamplerAddressMode::eRepeat, //
                                      0.0f,                            //
                                      false,                           //
                                      16.0f,                           //
                                      false,                           //
                                      vk::CompareOp::eNever,
                                      0.0f, //
                                      std::numeric_limits<float>::max(),
                                      vk::BorderColor::eFloatOpaqueBlack);

    auto&& res = logical_device.get().createSamplerUnique(samplerInfo);
    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create sampler");
    }
    m_sampler = std::move(res.value);
}

void
Texture::allocDescriptorSet(const LogicalDevice& logical_device,
                            const DescriptorManager& desc_manager)
{

    desc_manager.getAllocator(DescriptorSetLayoutType::COMBINED_IMAGE_SAMPLER)
        .allocate(logical_device, m_descriptor_set);

    vk::DescriptorImageInfo imageInfo(m_sampler.get(), m_image.view(),
                                      vk::ImageLayout::eShaderReadOnlyOptimal);
    vk::WriteDescriptorSet descriptorWrite(
        m_descriptor_set.get(), 0, 0, 1,
        vk::DescriptorType::eCombinedImageSampler, &imageInfo, nullptr,
        nullptr);

    logical_device.get().updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
}

void
Texture::bind(const vk::CommandBuffer& command_buffer,
              const vk::PipelineLayout& pipelayout) const
{
    command_buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                      pipelayout, 1u, 1,
                                      &(m_descriptor_set.get()), 0, nullptr);
}
} // namespace ars_graphics
