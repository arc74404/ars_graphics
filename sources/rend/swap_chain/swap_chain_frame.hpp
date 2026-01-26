#pragma once

#include <vector>

#include "../cmdbuf/command_buffer.hpp"
#include "../images/image_view.hpp"
#include "../render/render_context.hpp"
#include "../render/synchronization_data.hpp"
#include "../render_pass/renderpass_manager.hpp"
#include "../textures/image.hpp"

namespace ars_graphics
{
class SwapChain;
class DescriptorAllocator;

class SwapChainFrame final
{
public:
    SwapChainFrame(const LogicalDevice& logical_device,
                   const PhysicalDevice& physical_device,
                   const vk::Image& image,
                   vk::Format format,
                   vk::Format depth_format,
                   const vk::Extent2D& extent,
                   const RenderPassManager& renderpasses_manager);

    bool recreate(const LogicalDevice& logical_device,
                  const PhysicalDevice& physical_device,
                  const vk::Image& image,
                  vk::Format format,
                  vk::Format depth_format,
                  const vk::Extent2D& extent,
                  const RenderPassManager& renderpasses_manager);

    void shareContext(RenderCtx& context, RenderPassType renderpass_type) const;

    const vk::Framebuffer& getFramebuffer(RenderPassType renderpass_type) const;

    void destroy();

    // SynchronizationData m_sync;

private:
    ImageView m_view;

    // depth
    Image m_depth_image;

    //
    std::vector<vk::UniqueFramebuffer> m_framebuffers;

    //
    CommandBuffer m_command_buffer;
};

}; // namespace ars_graphics
