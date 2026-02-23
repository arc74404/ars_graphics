#pragma once
#include "../device/logical_device.hpp"
#include "../render_pass/render_pass.hpp"

#include "swap_chain_frame.hpp"

namespace ars_graphics
{
class SwapChain final
{
public:
    SwapChain(vk::Device logical_device,
              const PhysicalDevice& physical_device,
              vk::SurfaceKHR surface,
              vk::Format depth_format,
              vk::SurfaceFormatKHR surface_format,
              uint32_t width,
              uint32_t height);

    const vk::Extent2D& getExtent() const;

    operator vk::SwapchainKHR() const;

    void recreate(vk::Device logical_device);

    void destroy();

private:
    void setupSwapchain(vk::Device logical_device,
                        const PhysicalDevice& physical_device,
                        vk::SurfaceKHR surface,
                        vk::SurfaceFormatKHR surface_format,
                        uint32_t width,
                        uint32_t height);

    vk::UniqueSwapchainKHR m_swapchain;

    vk::Extent2D m_extent;
};
} // namespace ars_graphics
