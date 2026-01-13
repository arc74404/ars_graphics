#pragma once

#include "render_info.hpp"
#include "../device/physical_device.hpp"
#include "../device/logical_device.hpp"

namespace ars_graphics
{
class Renderable
{
public:
    virtual ~Renderable() = default;

    virtual RenderInfo calculateRenderInfo(const LogicalDevice& logical_device, 
        const PhysicalDevice& physical_device) const = 0;
};
} // namespace ars_graphics
