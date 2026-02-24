#pragma once

#include "../swap_chain/swap_chain_frame.hpp"

#include "resources_scene.hpp"

namespace ars_graphics
{
class RenderPipeline
{
public:
    virtual int getRenderPassIndex() const = 0;

    virtual void updatePerFrameData(const ResourceScene& scene) const = 0;

    virtual void beginCmd(vk::CommandBuffer cmd) const = 0;

    virtual void startRenderPass(vk::RenderPass renderpass,
                                 vk::Framebuffer framebuffer,
                                 const vk::Extent2D& extent) const = 0;

    virtual void setupScope(const vk::Extent2D& extent) const = 0;

    virtual void bindSharedData(
        const ResourceScene& scene) const = 0; // like camera

    virtual void drawPrimitives(const ResourceScene& scene) const = 0;
};
} // namespace ars_graphics
