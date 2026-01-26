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

    // std::pair<const SwapChainFrame&, uint32_t> currentFrame();

    const vk::SwapchainKHR& get() const;

    void recreate(const LogicalDevice& logical_device);

    void destroy();

    const SwapChainFrame& operator[](uint32_t index) const;

private:
    void setupSwapchain(const LogicalDevice& logical_device,
                        const PhysicalDevice& physical_device,
                        const vk::SurfaceKHR& surface,
                        const vk::SurfaceFormatKHR& surface_format,
                        uint32_t width,
                        uint32_t height);

    vk::UniqueSwapchainKHR m_swapchain;

    vk::Extent2D m_extent;

    std::vector<SwapChainFrame> m_frames;
};
} // namespace ars_graphics
