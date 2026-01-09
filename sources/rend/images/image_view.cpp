#include "image_view.hpp"

namespace ars_graphics
{
ImageView::ImageView(const LogicalDevice& logical_device,
                     const vk::Image& image,
                     const vk::Format& format,
                     const vk::ImageAspectFlags& image_aspects_flags)
{
    vk::ImageViewCreateInfo create_image_view_info{};
    create_image_view_info.image        = image;
    create_image_view_info.viewType     = vk::ImageViewType::e2D;
    create_image_view_info.format       = format;
    create_image_view_info.components.r = vk::ComponentSwizzle::eIdentity;
    create_image_view_info.components.g = vk::ComponentSwizzle::eIdentity;
    create_image_view_info.components.b = vk::ComponentSwizzle::eIdentity;
    create_image_view_info.components.a = vk::ComponentSwizzle::eIdentity;
    create_image_view_info.subresourceRange.aspectMask   = image_aspects_flags;
    create_image_view_info.subresourceRange.baseMipLevel = 0;
    create_image_view_info.subresourceRange.levelCount   = 1;
    create_image_view_info.subresourceRange.baseArrayLayer = 0;
    create_image_view_info.subresourceRange.layerCount     = 1;

    auto&& res =
        logical_device.get().createImageViewUnique(create_image_view_info);
    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create image view");
    }
    m_view = std::move(res.value);
}
vk::ImageView
ImageView::get() const
{
    return m_view.get();
}

} // namespace ars_graphics
