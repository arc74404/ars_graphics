#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "../device/logical_device.hpp"

namespace ars_graphics
{

struct ImageConfigInfo
{
    vk::Format format                = vk::Format::eR8G8B8A8Srgb;
    uint32_t width                   = 1;
    uint32_t height                  = 1;
    uint32_t depth                   = 1;
    uint32_t mip_levels              = 1;
    uint32_t array_layers            = 1;
    vk::ImageType type               = vk::ImageType::e2D;
    vk::ImageTiling tiling           = vk::ImageTiling::eOptimal;
    vk::SampleCountFlagBits samples  = vk::SampleCountFlagBits::e1;
    vk::ImageUsageFlags usage_flags  = vk::ImageUsageFlagBits::eSampled;
    vk::ImageAspectFlags aspect_mask = vk::ImageAspectFlagBits::eColor;
    vk::ImageLayout initial_layout   = vk::ImageLayout::eUndefined;
    vk::ImageCreateFlags flags       = {};
};

class Image
{
public:
    Image() = default;

    Image(const LogicalDevice& logical_device,
          const PhysicalDevice& physical_device,
          const ImageConfigInfo& config_info);

    vk::ImageMemoryBarrier memoryBarrier() const;

    void copyBufferToImage(const vk::Buffer& buffer,
                           const vk::CommandBuffer& command_buffer,
                           const vk::BufferImageCopy& region);

    const vk::ImageView& view() const;

private:
    void createImageView(const LogicalDevice& device,
                         vk::ImageAspectFlags aspect_mask);

    vk::UniqueImage m_image;
    vk::UniqueImageView m_view;
    vk::UniqueDeviceMemory m_memory;
    vk::Format m_format;
};
} // namespace ars_graphics
