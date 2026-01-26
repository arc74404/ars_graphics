#include "swap_chain_frame.hpp"

#include <iostream>

namespace ars_graphics
{
namespace
{
ImageConfigInfo
getDepthCongifInfo(const vk::Extent2D& extent, vk::Format depth_format)
{
    ImageConfigInfo config_info = {
        .format      = depth_format,
        .width       = extent.width,
        .height      = extent.height,
        .usage_flags = vk::ImageUsageFlagBits::eDepthStencilAttachment |
                       vk::ImageUsageFlagBits::eTransferDst |
                       vk::ImageUsageFlagBits::eSampled,
        .aspect_mask = vk::ImageAspectFlagBits::eDepth};
    return config_info;
}
} // namespace

SwapChainFrame::SwapChainFrame(const LogicalDevice& logical_device,
                               const PhysicalDevice& physical_device,
                               const vk::Image& image,
                               vk::Format format,
                               vk::Format depth_format,
                               const vk::Extent2D& extent,
                               const RenderPassManager& renderpasses_manager)
    : m_view(logical_device, image, format, vk::ImageAspectFlagBits::eColor),
      m_depth_image(logical_device,
                    physical_device,
                    getDepthCongifInfo(extent, depth_format)),
      m_framebuffers(
          renderpasses_manager.generateFramebuffers(logical_device,
                                                    extent,
                                                    m_view.get(),
                                                    m_depth_image.view())),
      m_command_buffer(logical_device)
{
}

bool
SwapChainFrame::recreate(const LogicalDevice& logical_device,
                         const PhysicalDevice& physical_device,
                         const vk::Image& image,
                         vk::Format format,
                         vk::Format depth_format,
                         const vk::Extent2D& extent,
                         const RenderPassManager& renderpasses_manager)
{
    try
    {
        SwapChainFrame temp{
            logical_device, physical_device,     image, format, depth_format,
            extent,         renderpasses_manager};
        std::swap(*this, temp);
        return true;
    }
    catch (std::runtime_error& error)
    {
        std::cout << error.what() << '\n';
        return false;
    }
}

const vk::Framebuffer&
SwapChainFrame::getFramebuffer(RenderPassType renderpass_type) const
{
    return m_framebuffers[getRenderPassIndex(renderpass_type)].get();
}

void
SwapChainFrame::shareContext(RenderCtx& context,
                             RenderPassType renderpass_type) const
{
    context.cmd = &m_command_buffer.get();
}

void
SwapChainFrame::destroy()
{
    m_command_buffer.destroy();
}

}; // namespace ars_graphics
