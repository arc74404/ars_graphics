#include "swap_chain_frame.hpp"

#include <iostream>

namespace ars_graphics
{
namespace
{
ImageConfigInfo
getDepthConfigInfo(const vk::Extent2D& extent, vk::Format depth_format)
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

SwapChainFrame::SwapChainFrame(vk::UniqueCommandBuffer&& cmd,
                               FrameBuffer&& framebuffer)
    : m_command_buffer(std::move(cmd)), m_framebuffer(std::move(framebuffer))
{
}

bool
SwapChainFrame::recreate(vk::UniqueCommandBuffer&& cmd,
                         FrameBuffer&& framebuffer)
{
    try
    {
        SwapChainFrame temp{std::move(cmd), std::move(framebuffer)};
        std::swap(*this, temp);
        return true;
    }
    catch (std::runtime_error& error)
    {
        std::cout << error.what() << '\n';
        return false;
    }
}

vk::Framebuffer
SwapChainFrame::getFramebuffer() const noexcept
{
    return m_framebuffer.m_framebuffer.get();
}

vk::CommandBuffer
SwapChainFrame::getCmd() const noexcept
{
    return m_command_buffer.get();
}

void
SwapChainFrame::destroyCmd() noexcept
{
    m_command_buffer.reset();
}

}; // namespace ars_graphics
