#pragma once

#include "../device/logical_device.hpp"

#include "render_pass_config_info.hpp"

namespace ars_graphics
{

struct RenderPassCreater;

class RenderPass final
{
public:
    RenderPass(RenderPassCreater& creater,
               const RenderPassConfigInfo& config_info);

    vk::RenderPass get() const;

    void begin(const vk::CommandBuffer& cmd,
               const vk::Framebuffer& framebuffer,
               const vk::Extent2D& extent);

    void end(const vk::CommandBuffer& cmd);

private:
    vk::UniqueRenderPass m_render_pass;
};
} // namespace ars_graphics
