#pragma once

#include "../device/logical_device.hpp"

namespace ars_graphics
{

std::optional<vk::UniqueImageView>
createImageView(vk::Device logical_device,
                const vk::Image& image,
                const vk::Format& format,
                const vk::ImageAspectFlags& image_aspects_flags);

} // namespace ars_graphics
