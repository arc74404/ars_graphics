#pragma once

#include <vector>

#include "../images/image_view.hpp"
#include "../render/synchronization_data.hpp"
#include "../render_pass/renderpass_manager.hpp"
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

}; // namespace ars_graphics
