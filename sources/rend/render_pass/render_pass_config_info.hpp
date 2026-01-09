#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS

#include <vulkan/vulkan.hpp>

namespace ars_graphics
{
struct RenderPassConfigInfo final
{
    vk::Format color_format;
    vk::Format depth_format;
    bool clear_color;
    bool clear_depth;
};
} // namespace ars_graphics
