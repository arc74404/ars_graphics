#pragma once

#include <vector>

#include "../images/image_view.hpp"
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

private:
    ImageView m_view;

    // depth
    Image m_depth_image;

    //
    std::vector<vk::UniqueFramebuffer> m_framebuffers;
};

}; // namespace ars_graphics
