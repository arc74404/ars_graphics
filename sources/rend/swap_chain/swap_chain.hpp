#pragma once
#include "../device/logical_device.hpp"
#include "../render_pass/render_pass.hpp"
#include "../render_pass/renderpass_manager.hpp"

#include "swap_chain_frame.hpp"

namespace ars_graphics
{
class SwapChain final
{
public:
    SwapChain(const LogicalDevice& logical_device,
              const PhysicalDevice& physical_device,
              const vk::SurfaceKHR& surface,
              uint32_t width,
              uint32_t height);

    size_t countFrames() const;

    void destroy();

private:
    void setupSwapchain(const LogicalDevice& logical_device,
                        const PhysicalDevice& physical_device,
                        const vk::SurfaceKHR& surface,
                        uint32_t width,
                        uint32_t height);
    vk::Format m_depth_format;
    vk::SurfaceFormatKHR m_surface_format;

    vk::UniqueSwapchainKHR m_swapchain;

    vk::Extent2D m_extent;

    RenderPassManager m_renderpass_manager;

    std::vector<SwapChainFrame> m_frames;
};
} // namespace ars_graphics
