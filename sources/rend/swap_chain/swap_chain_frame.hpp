#pragma once

#include <vector>

#include "../cmdbuf/command_pool.hpp"
#include "../images/image_view.hpp"
#include "../textures/image.hpp"

namespace ars_graphics
{
class SwapChain;
class DescriptorAllocator;

struct FrameBuffer
{
    vk::UniqueFramebuffer m_framebuffer;
    vk::UniqueImageView view;
    Image depth_image;
};

class SwapChainFrame final
{
public:
    SwapChainFrame(vk::UniqueCommandBuffer&& cmd, FrameBuffer&& framebuffer);

    bool recreate(vk::UniqueCommandBuffer&& cmd, FrameBuffer&& framebuffer);

    vk::CommandBuffer getCmd() const noexcept;

    vk::Framebuffer getFramebuffer() const noexcept;

    void destroyCmd() noexcept;

private:
    //
    FrameBuffer m_framebuffer;

    //
    vk::UniqueCommandBuffer m_command_buffer;
};

vk::UniqueFramebuffer
createFramebuffer(vk::Device device,
                  const vk::Extent2D& extent,
                  vk::RenderPass render_pass,
                  vk::ImageView view,
                  vk::ImageView depth_view);

std::vector<SwapChainFrame>
createFrames(vk::Device device,
             const PhysicalDevice& physical_device,
             vk::SwapchainKHR swapchain,
             const vk::Extent2D& swapchain_extent,
             const CommandPool& command_pool,
             vk::RenderPass render_pass,
             vk::Format depth_format,
             vk::Format sformat);

}; // namespace ars_graphics
