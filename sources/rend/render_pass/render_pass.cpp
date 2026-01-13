#include "render_pass.hpp"

#include "render_pass_creater.hpp"

namespace ars_graphics
{

RenderPass::RenderPass(RenderPassCreater& creater,
                       const RenderPassConfigInfo& config_info)
{
    m_render_pass = creater.createRenderPass(config_info);
}

const vk::RenderPass&
RenderPass::get() const
{
    return m_render_pass.get();
}

// void
// RenderPass::begin(const vk::CommandBuffer& cmd,
//                   const vk::Framebuffer& framebuffer,
//                   const vk::Extent2D& extent)
// {
//     cmd.reset();

//     vk::CommandBufferBeginInfo beginInfo = {};

//     cmd.begin(beginInfo);

//     vk::RenderPassBeginInfo renderPassInfo = {};
//     renderPassInfo.renderPass              = m_render_pass.get();
//     renderPassInfo.framebuffer             = framebuffer;

//     renderPassInfo.renderArea.offset.x = 0;
//     renderPassInfo.renderArea.offset.y = 0;
//     renderPassInfo.renderArea.extent   = extent;

//     std::array<vk::ClearValue, 2> clear_values{};

//     clear_values[0].color        = vk::ClearColorValue{0.4f, 0.3f,
//     0.5f, 1.0f}; clear_values[1].depthStencil =
//     vk::ClearDepthStencilValue{1.0f, 0};

//     renderPassInfo.clearValueCount = clear_values.size();

//     renderPassInfo.pClearValues = clear_values.data();

//     cmd.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
// }

// void
// RenderPass::end(const vk::CommandBuffer& cmd)
// {
//     cmd.endRenderPass();

//     cmd.end();
// }
} // namespace ars_graphics
