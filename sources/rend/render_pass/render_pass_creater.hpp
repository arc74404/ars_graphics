#pragma once
#include <optional>

#include "../device/logical_device.hpp"

#include "render_pass_config_info.hpp"

namespace ars_graphics
{
class RenderPassBuilder
{
public:
    virtual ~RenderPassBuilder() = default;

    RenderPassBuilder(vk::Device);

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
    vk::Device m_device;
};

std::vector<vk::UniqueRenderPass>
createRenderPasses(
    vk::Device device,
    const std::vector<RenderPassConfigInfo>& render_pass_configs);

} // namespace ars_graphics
