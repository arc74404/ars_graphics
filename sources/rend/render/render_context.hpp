#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

namespace ars_graphics
{
struct RenderCtx
{
    const vk::CommandBuffer* cmd;
    const vk::Framebuffer* framebuffer;
};
}