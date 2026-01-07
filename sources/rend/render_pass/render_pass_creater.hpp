#pragma once
#include <optional>

#include "../device/logical_device.hpp"

#include "render_pass_config_info.hpp"

namespace ars_graphics
{
class RenderPassCreater
{
public:
    virtual ~RenderPassCreater() = default;

    void init(const RenderPassConfigInfo& config_info);

    vk::UniqueRenderPass createRenderPass(const LogicalDevice& device);

private:
    virtual vk::AttachmentDescription colorAttachment() const;
    virtual vk::AttachmentReference colorAttachmentRef() const;

    virtual vk::AttachmentDescription depthAttachment() const;
    virtual vk::AttachmentReference depthAttachmentRef() const;

    virtual vk::SubpassDescription subpassGenerate() const;

    virtual vk::RenderPassCreateInfo generateRenderPassInfo() const;

private:
    RenderPassConfigInfo m_config_info;

    vk::AttachmentDescription m_color_attachment;

    vk::AttachmentReference m_color_attachment_ref;

    vk::AttachmentDescription m_depth_attachment;

    vk::AttachmentReference m_depth_attachment_ref;

    vk::SubpassDescription m_subpass;

    // final info
    vk::RenderPassCreateInfo m_renderpass_info;
};
} // namespace ars_graphics
