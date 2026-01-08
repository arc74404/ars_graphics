#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

namespace ars_graphics
{
class IWindow
{
public:
    virtual ~IWindow() = default;

    virtual vk::UniqueSurfaceKHR createSurface(
        const vk::Instance& instance) = 0;
};
} // namespace ars_graphics
