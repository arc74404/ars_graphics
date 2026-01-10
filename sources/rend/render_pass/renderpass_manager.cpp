#include "renderpass_manager.hpp"

#include "render_pass_creater.hpp"

size_t
ars_graphics::getRenderPassIndex(RenderPassType type)
{
    return static_cast<size_t>(type);
}

namespace ars_graphics
{

std::vector<vk::UniqueFramebuffer>
RenderPassManager::generateFramebuffers(const LogicalDevice& device,
                                        const vk::Extent2D& extent,
                                        const vk::ImageView& view,
                                        const vk::ImageView& depth_view) const
{
    std::vector<vk::UniqueFramebuffer> res;

    std::vector<vk::ImageView> attachments = {view, depth_view};

    for (auto&& render_pass : m_render_passes)
    {
        vk::UniqueFramebuffer framebuffer;

        vk::FramebufferCreateInfo config_info;

        config_info.flags           = vk::FramebufferCreateFlags();
        config_info.renderPass      = render_pass->get();
        config_info.attachmentCount = attachments.size();
        config_info.pAttachments    = attachments.data();
        config_info.width           = extent.width;
        config_info.height          = extent.height;
        config_info.layers          = 1;

        auto&& frmbuf = device.get().createFramebufferUnique(config_info);

        if (frmbuf.result != vk::Result::eSuccess)
        {
            throw std::runtime_error("Failed create framebuffer");
        }

        res.emplace_back(std::move(frmbuf.value));
    }

    return res;
}

RenderPassManager::RenderPassManager(const LogicalDevice& device,
                                     vk::Format color_format,
                                     vk::Format depth_format)
    : m_creater(device)
{
    m_render_passes.resize(
        getRenderPassIndex(RenderPassType::RENDERPASSTYPE_COUNT));

    m_render_passes[getRenderPassIndex(RenderPassType::STANDART)] =
        std::make_unique<RenderPass>(
            m_creater, RenderPassConfigInfo{.color_format = color_format,
                                            .depth_format = depth_format,
                                            .clear_color  = true,
                                            .clear_depth  = true});
}

const vk::RenderPass&
RenderPassManager::getRenderPass(RenderPassType type) const
{
    return m_render_passes[getRenderPassIndex(type)]->get();
}

} // namespace ars_graphics
