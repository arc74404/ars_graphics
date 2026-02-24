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

vk::UniqueFramebuffer
ars_graphics::createFramebuffer(vk::Device device,
                                const vk::Extent2D& extent,
                                vk::RenderPass render_pass,
                                vk::ImageView view,
                                vk::ImageView depth_view)
{
    std::vector<vk::ImageView> attachments = {view, depth_view};

    vk::FramebufferCreateInfo config_info;

    config_info.flags           = vk::FramebufferCreateFlags();
    config_info.renderPass      = render_pass;
    config_info.attachmentCount = attachments.size();
    config_info.pAttachments    = attachments.data();
    config_info.width           = extent.width;
    config_info.height          = extent.height;
    config_info.layers          = 1;

    auto&& frmbuf = device.createFramebufferUnique(config_info);

    if (frmbuf.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create framebuffer");
    }

    return std::move(frmbuf.value);
}

std::vector<ars_graphics::SwapChainFrame>
ars_graphics::createFrames(vk::Device device,
                           const PhysicalDevice& physical_device,
                           vk::SwapchainKHR swapchain,
                           const vk::Extent2D& swapchain_extent,
                           const ars_graphics::CommandPool& command_pool,
                           vk::RenderPass render_pass,
                           vk::Format depth_format,
                           vk::Format sformat)
{
    std::vector<SwapChainFrame> res;

    auto&& images = device.getSwapchainImagesKHR(swapchain);

    if (false == images.has_value())
    {
        throw std::runtime_error("Failed getSwapchainImagesKHR in swapchain");
    }

    for (auto&& image : images.value)
    {
        FrameBuffer framebuffer;
        framebuffer.depth_image = Image(
            device, physical_device,
            ImageConfigInfo{
                .format      = depth_format,
                .width       = swapchain_extent.width,
                .height      = swapchain_extent.height,
                .usage_flags = vk::ImageUsageFlagBits::eDepthStencilAttachment |
                               vk::ImageUsageFlagBits::eTransferDst |
                               vk::ImageUsageFlagBits::eSampled,
                .aspect_mask = vk::ImageAspectFlagBits::eDepth});

        auto&& image_view_res = createImageView(
            device, image, sformat, vk::ImageAspectFlagBits::eColor);

        framebuffer.view          = std::move(image_view_res.value());
        framebuffer.m_framebuffer = createFramebuffer(
            device, swapchain_extent, render_pass, framebuffer.view.get(),
            framebuffer.depth_image.view());

        res.emplace_back(command_pool.allocateCommandBuffer(device),
                         std::move(framebuffer));
    }
    return res;
}
