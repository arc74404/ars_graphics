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

    RenderPassCreater(const LogicalDevice& device);

    vk::UniqueRenderPass createRenderPass(
        const RenderPassConfigInfo& config_info);

private:
    virtual vk::AttachmentDescription colorAttachment(
        const RenderPassConfigInfo& config_info) const;
    virtual vk::AttachmentReference colorAttachmentRef(
        const RenderPassConfigInfo& config_info) const;

    virtual vk::AttachmentDescription depthAttachment(
        const RenderPassConfigInfo& config_info) const;
    virtual vk::AttachmentReference depthAttachmentRef(
        const RenderPassConfigInfo& config_info) const;

    virtual vk::SubpassDescription subpassGenerate(
        const vk::AttachmentReference& color,
        const vk::AttachmentReference& depth) const;

private:
    const LogicalDevice& m_device;
};
} // namespace ars_graphics
