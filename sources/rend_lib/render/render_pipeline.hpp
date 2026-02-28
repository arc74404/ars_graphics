#pragma once

#include "../swap_chain/swap_chain_frame.hpp"

#include "resources_scene.hpp"

namespace ars_graphics
{
template <typename ResourceSceneType>
class RenderPipeline
{
public:
    // void updatePerFrameData(ResourceSceneType& scene,
    //                         uint32_t frame_index) const;

    // void beginCmd(vk::CommandBuffer cmd) const;

    // void startRenderPass(vk::RenderPass renderpass,
    //                      vk::Framebuffer framebuffer,
    //                      const vk::Extent2D& extent) const;

    // void setupScope(const vk::Extent2D& extent) const;

    // void bindSharedData(const ResourceSceneType& scene) const;

    // void drawPrimitives(const ResourceSceneType& scene) const;
};
} // namespace ars_graphics
