#include "render_pass_creater.hpp"

#include <iostream>

#include "../util_funcs.hpp"

namespace ars_graphics
{

RenderPassBuilder::RenderPassBuilder(vk::Device device) : m_device(device)
{
}

vk::UniqueRenderPass
RenderPassBuilder::createRenderPass(const RenderPassConfigInfo& config_info)
{
    auto&& color_attachment     = colorAttachment(config_info);
    auto&& color_attachment_ref = colorAttachmentRef(config_info);

    auto&& depth_attachment     = depthAttachment(config_info);
    auto&& depth_attachment_ref = depthAttachmentRef(config_info);

    auto&& subpass =
        subpassGenerate(color_attachment_ref, depth_attachment_ref);

    vk::RenderPassCreateInfo renderpass_info{};
    renderpass_info.flags                   = vk::RenderPassCreateFlags();
    renderpass_info.attachmentCount         = 2;
    vk::AttachmentDescription attachments[] = {color_attachment,
                                               depth_attachment};

    renderpass_info.pAttachments = attachments;
    renderpass_info.subpassCount = 1;
    renderpass_info.pSubpasses   = &subpass;

    auto&& res = m_device.createRenderPassUnique(renderpass_info);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create render pass");
    }
    return std::move(res.value);
}

vk::AttachmentDescription
RenderPassBuilder::colorAttachment(
    const RenderPassConfigInfo& config_info) const
{
    vk::AttachmentDescription color_attachment{};
    color_attachment.flags   = vk::AttachmentDescriptionFlags();
    color_attachment.format  = config_info.color_format;
    color_attachment.samples = vk::SampleCountFlagBits::e1;

    color_attachment.loadOp = config_info.clear_color
                                  ? vk::AttachmentLoadOp::eClear
                                  : vk::AttachmentLoadOp::eDontCare;

    color_attachment.storeOp        = vk::AttachmentStoreOp::eStore;
    color_attachment.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare;
    color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    color_attachment.initialLayout  = vk::ImageLayout::eUndefined;
    color_attachment.finalLayout    = vk::ImageLayout::ePresentSrcKHR;

    return color_attachment;
}
vk::AttachmentReference
RenderPassBuilder::colorAttachmentRef(
    const RenderPassConfigInfo& config_info) const
{
    vk::AttachmentReference color_attachment_ref{};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout     = vk::ImageLayout::eColorAttachmentOptimal;

    return color_attachment_ref;
}

vk::AttachmentDescription
RenderPassBuilder::depthAttachment(
    const RenderPassConfigInfo& config_info) const
{
    vk::AttachmentDescription depth_attachment{};
    depth_attachment.format  = config_info.depth_format;
    depth_attachment.samples = vk::SampleCountFlagBits::e1;

    depth_attachment.loadOp = config_info.clear_depth
                                  ? vk::AttachmentLoadOp::eClear
                                  : vk::AttachmentLoadOp::eDontCare;

    depth_attachment.storeOp        = vk::AttachmentStoreOp::eStore;
    depth_attachment.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare;
    depth_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depth_attachment.initialLayout  = vk::ImageLayout::eUndefined;
    depth_attachment.finalLayout =
        vk::ImageLayout::eDepthStencilAttachmentOptimal;

    return depth_attachment;
}
vk::AttachmentReference
RenderPassBuilder::depthAttachmentRef(
    const RenderPassConfigInfo& config_info) const
{
    vk::AttachmentReference depth_attachment_ref{};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout =
        vk::ImageLayout::eDepthStencilAttachmentOptimal;

    return depth_attachment_ref;
}

vk::SubpassDescription
RenderPassBuilder::subpassGenerate(const vk::AttachmentReference& color,
                                   const vk::AttachmentReference& depth) const
{
    vk::SubpassDescription subpass{};
    subpass.flags                   = vk::SubpassDescriptionFlags();
    subpass.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &color;
    subpass.pDepthStencilAttachment = &depth;
    return subpass;
}

} // namespace ars_graphics

std::vector<vk::UniqueRenderPass>
ars_graphics::createRenderPasses(
    vk::Device device,
    const std::vector<RenderPassUserConfigInfo>& render_pass_configs,
    vk::Format color,
    vk::Format depth)
{
    std::vector<vk::UniqueRenderPass> render_passes;

    RenderPassBuilder render_pass_builder(device);

    for (auto&& config : render_pass_configs)
    {
        render_passes.emplace_back(
            std::move(render_pass_builder.createRenderPass(
                {.color_format = color,
                 .depth_format = depth,
                 .clear_color  = config.clear_color,
                 .clear_depth  = config.clear_depth})));
    }

    return render_passes;
}
