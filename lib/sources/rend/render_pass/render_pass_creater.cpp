#include "render_pass_creater.hpp"

#include <iostream>

#include "../util_funcs.hpp"

namespace ars_graphics
{

void
RenderPassCreater::init(const RenderPassConfigInfo& config_info)
{
    m_config_info = config_info;

    using rpc = RenderPassCreater;
    fillBoxesByPipeline(
        this, //----- color_attach ----//
        std::make_pair(&m_color_attachment, &rpc::colorAttachment),
        //----- color_ref ----//
        std::make_pair(&m_color_attachment_ref, &rpc::colorAttachmentRef),
        //---- depth_attach -----//
        std::make_pair(&m_depth_attachment, &rpc::depthAttachment),
        //----- depth_ref ----//
        std::make_pair(&m_depth_attachment_ref, &rpc::depthAttachmentRef),
        //----- subpass----//
        std::make_pair(&m_subpass, &rpc::subpassGenerate),
        //----- generate render pass info ----//
        std::make_pair(&m_renderpass_info, &rpc::generateRenderPassInfo));
}

vk::UniqueRenderPass
RenderPassCreater::createRenderPass(const LogicalDevice& device)
{
    auto&& res = device.get().createRenderPassUnique(m_renderpass_info);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create render pass");
    }
    return std::move(res.value);
}

vk::AttachmentDescription
RenderPassCreater::colorAttachment() const
{
    vk::AttachmentDescription color_attachment{};
    color_attachment.flags          = vk::AttachmentDescriptionFlags();
    color_attachment.format         = m_config_info.swap_chain_format;
    color_attachment.samples        = vk::SampleCountFlagBits::e1;
    color_attachment.loadOp         = vk::AttachmentLoadOp::eClear;
    color_attachment.storeOp        = vk::AttachmentStoreOp::eStore;
    color_attachment.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare;
    color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    color_attachment.initialLayout  = vk::ImageLayout::eUndefined;
    color_attachment.finalLayout    = vk::ImageLayout::ePresentSrcKHR;

    return color_attachment;
}
vk::AttachmentReference
RenderPassCreater::colorAttachmentRef() const
{
    vk::AttachmentReference color_attachment_ref{};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout     = vk::ImageLayout::eColorAttachmentOptimal;

    return color_attachment_ref;
}

vk::AttachmentDescription
RenderPassCreater::depthAttachment() const
{
    vk::AttachmentDescription depth_attachment{};
    depth_attachment.format        = vk::Format::eD32Sfloat;
    depth_attachment.loadOp        = vk::AttachmentLoadOp::eClear;
    depth_attachment.storeOp       = vk::AttachmentStoreOp::eDontCare;
    depth_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    depth_attachment.initialLayout = vk::ImageLayout::eUndefined;
    depth_attachment.finalLayout =
        vk::ImageLayout::eDepthStencilAttachmentOptimal;

    return depth_attachment;
}
vk::AttachmentReference
RenderPassCreater::depthAttachmentRef() const
{
    vk::AttachmentReference depth_attachment_ref{};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout =
        vk::ImageLayout::eDepthStencilAttachmentOptimal;

    return depth_attachment_ref;
}

vk::SubpassDescription
RenderPassCreater::subpassGenerate() const
{
    vk::SubpassDescription subpass{};
    subpass.flags                   = vk::SubpassDescriptionFlags();
    subpass.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount    = 1;
    subpass.pColorAttachments       = &m_color_attachment_ref;
    subpass.pDepthStencilAttachment = &m_depth_attachment_ref;
    return subpass;
}

vk::RenderPassCreateInfo
RenderPassCreater::generateRenderPassInfo() const
{
    vk::RenderPassCreateInfo renderpass_info{};
    renderpass_info.flags                   = vk::RenderPassCreateFlags();
    renderpass_info.attachmentCount         = 2;
    vk::AttachmentDescription attachments[] = {m_color_attachment,
                                               m_depth_attachment};

    renderpass_info.pAttachments = attachments;
    renderpass_info.subpassCount = 1;
    renderpass_info.pSubpasses   = &m_subpass;

    return renderpass_info;
}

} // namespace ars_graphics
