#pragma once

#include "render_pipeline.hpp"
#include "render_setuper.hpp"

namespace ars_graphics
{

struct RenderContext
{
    uint32_t render_pass_index;
};

template <typename ResourceSceneType>
void
render(const RenderContext& ctx,
       RenderSetuper& setuper,
       const RenderPipeline<ResourceSceneType>& pipeline,
       const ResourceSceneType& scene)
// render_setuper - has all objects than take part in rendering
// pipeline - interface that user shpould rewrite with steps
// scene - has all resources, user defined
{
    uint32_t frame_number = setuper.getFrameNumber();
    setuper.waitFence(frame_number);

    vk::ResultValue<uint32_t> acquire = setuper.acquireNextImage(frame_number);

    pipeline.updatePerFrameData(scene);

    const SwapChainFrame& frame_data =
        setuper.getFrame(frame_number, ctx.render_pass_index);

    vk::CommandBuffer cmd = frame_data.getCmd();

    pipeline.beginCmd(cmd);

    vk::RenderPass render_pass = setuper.getRenderPass(ctx.render_pass_index);

    const vk::Extent2D& extent = setuper.getExtent();

    vk::Framebuffer framebuf =
        setuper.getFrame(acquire.value, ctx.render_pass_index).getFramebuffer();

    pipeline.startRenderPass(render_pass, framebuf, extent);

    pipeline.setupScope(extent);

    pipeline.bindSharedData(scene);

    pipeline.drawPrimitives(scene);

    cmd.endRenderPass();

    cmd.end();

    setuper.endRender(acquire.value, cmd);
}
} // namespace ars_graphics
