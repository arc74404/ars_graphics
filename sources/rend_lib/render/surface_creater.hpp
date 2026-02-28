#pragma once

#include "../libs_includes/vulkan.hpp"

namespace ars_graphics
{
struct SurfaceCreater
{
    virtual vk::SurfaceKHR createSurface(const vk::Instance& instance) = 0;
};
} // namespace ars_graphics
