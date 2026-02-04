#pragma once

#include "../device/logical_device.hpp"
#include "../device/physical_device.hpp"

#include "render_info.hpp"

namespace ars_graphics
{
class Renderable
{
public:
    virtual ~Renderable() = default;

    virtual RenderInfo calculateRenderInfo(
        const LogicalDevice& logical_device,
        const PhysicalDevice& physical_device) = 0;
};
} // namespace ars_graphics
