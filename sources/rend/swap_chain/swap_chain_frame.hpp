#pragma once

#include <vector>

#include "../cmdbuf/command_buffer.hpp"
#include "../images/image_view.hpp"
#include "../render/render_context.hpp"
#include "../render_pass/renderpass_manager.hpp"
#include "../textures/image.hpp"

#include "synchronization_data.hpp"

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

    const SynchronizationData& getSynchronization() const;

    void destroy();

private:
    SynchronizationData m_synchronization;

    ImageView m_view;

    // depth
    Image m_depth_image;

    //
    std::vector<vk::UniqueFramebuffer> m_framebuffers;

    //
    CommandBuffer m_command_buffer;
};

}; // namespace ars_graphics
