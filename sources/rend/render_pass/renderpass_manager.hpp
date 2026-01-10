#pragma once
#include <memory>

#include "render_pass.hpp"
#include "render_pass_creater.hpp"

namespace ars_graphics
{

enum class RenderPassType
{
    STANDART,
    RENDERPASSTYPE_COUNT
};

size_t
getRenderPassIndex(RenderPassType type);

class RenderPassManager final
{
public:
    RenderPassManager(const LogicalDevice& device,
                      vk::Format color_format,
                      vk::Format depth_format);

    std::vector<vk::UniqueFramebuffer> generateFramebuffers(
        const LogicalDevice& device,
        const vk::Extent2D& extent,
        const vk::ImageView& view,
        const vk::ImageView& depth_view) const;

    const vk::RenderPass& getRenderPass(RenderPassType type) const;

private:
    RenderPassCreater m_creater;

    std::vector<std::unique_ptr<RenderPass>> m_render_passes;
};
} // namespace ars_graphics
