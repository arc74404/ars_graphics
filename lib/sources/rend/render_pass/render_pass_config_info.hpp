#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS

#include <vulkan/vulkan.hpp>

namespace ars_graphics
{
struct RenderPassConfigInfo final
{
    vk::Format swap_chain_format;
};
} // namespace ars_graphics
