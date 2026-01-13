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
              const RenderPassManager& renderpass_manager,
              const vk::Format& depth_format,
              const vk::SurfaceFormatKHR& surface_format,
              uint32_t width,
              uint32_t height);

    size_t countFrames() const;

    const vk::Extent2D getExtent() const;

    const SwapChainFrame& currentFrame();

    const vk::SwapchainKHR& get() const;

    void destroy();

private:
    void setupSwapchain(const LogicalDevice& logical_device,
                        const PhysicalDevice& physical_device,
                        const vk::SurfaceKHR& surface,
                        const vk::SurfaceFormatKHR& surface_format,
                        uint32_t width,
                        uint32_t height);

    uint32_t m_current_frame_index = 0;

    vk::UniqueSwapchainKHR m_swapchain;

    vk::Extent2D m_extent;

    uint32_t m_count_frames;

    std::vector<SwapChainFrame> m_frames;
};
} // namespace ars_graphics
