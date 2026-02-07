#include "image.hpp"

namespace ars_graphics
{

Image::Image(const LogicalDevice& logical_device,
             const PhysicalDevice& physical_device,
             const ImageConfigInfo& config_info)
{
    m_format = config_info.format;

    // Image
    {
        vk::ImageCreateInfo image_info{};
        image_info.imageType = config_info.type;
        image_info.extent = vk::Extent3D(config_info.width, config_info.height,
                                         config_info.depth);
        image_info.mipLevels     = config_info.mip_levels;
        image_info.arrayLayers   = config_info.array_layers;
        image_info.format        = config_info.format;
        image_info.tiling        = config_info.tiling;
        image_info.initialLayout = config_info.initial_layout;
        image_info.usage         = config_info.usage_flags;
        image_info.samples       = config_info.samples;
        image_info.sharingMode   = vk::SharingMode::eExclusive;
        image_info.flags         = config_info.flags;

        auto&& res = logical_device.get().createImageUnique(image_info);

        if (res.result != vk::Result::eSuccess)
        {
            throw std::runtime_error("Failed create Image");
        }

        m_image = std::move(res.value);
    }

    // Mem
    {
        vk::MemoryRequirements image_mem_requirements =
            logical_device.get().getImageMemoryRequirements(m_image.get());

        vk::MemoryAllocateInfo image_alloc_info(
            image_mem_requirements.size,
            physical_device.findMemoryTypeIndex(
                image_mem_requirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal));

        auto&& res =
            logical_device.get().allocateMemoryUnique(image_alloc_info);
        if (res.result != vk::Result::eSuccess)
        {
            throw std::runtime_error("Failed allocate memory in image");
        }
        m_memory = std::move(res.value);
        logical_device.get().bindImageMemory(m_image.get(), m_memory.get(), 0);
    }
    createImageView(logical_device, config_info.aspect_mask);
}

void
Image::createImageView(const LogicalDevice& device,
                       vk::ImageAspectFlags aspect_mask)
{
    vk::ImageViewCreateInfo create_image_view_info{};
    create_image_view_info.image        = m_image.get();
    create_image_view_info.viewType     = vk::ImageViewType::e2D;
    create_image_view_info.format       = m_format;
    create_image_view_info.components.r = vk::ComponentSwizzle::eIdentity;
    create_image_view_info.components.g = vk::ComponentSwizzle::eIdentity;
    create_image_view_info.components.b = vk::ComponentSwizzle::eIdentity;
    create_image_view_info.components.a = vk::ComponentSwizzle::eIdentity;
    create_image_view_info.subresourceRange.aspectMask     = aspect_mask;
    create_image_view_info.subresourceRange.baseMipLevel   = 0;
    create_image_view_info.subresourceRange.levelCount     = 1;
    create_image_view_info.subresourceRange.baseArrayLayer = 0;
    create_image_view_info.subresourceRange.layerCount     = 1;

    auto&& res = device.get().createImageViewUnique(create_image_view_info);
    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create image view");
    }
    m_view = std::move(res.value);
}

vk::ImageMemoryBarrier
Image::memoryBarrier() const
{
    return vk::ImageMemoryBarrier(
        {}, vk::AccessFlagBits::eTransferWrite, vk::ImageLayout::eUndefined,
        vk::ImageLayout::eTransferDstOptimal, VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED, m_image.get(),
        {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1});
}

void
Image::copyBufferToImage(const vk::Buffer& buffer,
                         const vk::CommandBuffer& command_buffer,
                         const vk::BufferImageCopy& region)
{
    command_buffer.copyBufferToImage(buffer, m_image.get(),
                                     vk::ImageLayout::eTransferDstOptimal, 1,
                                     &region);
}

const vk::ImageView&
Image::view() const
{
    return m_view.get();
}
} // namespace ars_graphics
