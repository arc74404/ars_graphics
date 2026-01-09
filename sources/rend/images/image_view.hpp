#pragma once

#include "../device/logical_device.hpp"

namespace ars_graphics
{
class ImageView
{
public:
    ImageView(const LogicalDevice& logical_device,
              const vk::Image& image,
              const vk::Format& format,
              const vk::ImageAspectFlags& image_aspects_flags);

    vk::ImageView get() const;

private:
    vk::UniqueImageView m_view;
};
} // namespace ars_graphics
