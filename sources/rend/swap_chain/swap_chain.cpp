#include "swap_chain.hpp"

#include "../render_pass/render_pass_creater.hpp"

namespace ars_graphics
{

namespace
{

vk::PresentModeKHR
choosePresentMode(
    const std::vector<vk::PresentModeKHR>& available_present_modes)
{
    for (vk::PresentModeKHR presentMode : available_present_modes)
    {
        if (presentMode == vk::PresentModeKHR::eMailbox)
        {
            return presentMode;
        }
    }
    for (vk::PresentModeKHR presentMode : available_present_modes)
    {
        if (presentMode == vk::PresentModeKHR::eImmediate)
        {
            return presentMode;
        }
    }

    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D
chooseExtent(uint32_t width,
             uint32_t height,
             vk::SurfaceCapabilitiesKHR capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else
    {
        vk::Extent2D extent = {width, height};

        extent.width =
            std::min(capabilities.maxImageExtent.width,
                     std::max(capabilities.minImageExtent.width, extent.width));

        extent.height = std::min(
            capabilities.maxImageExtent.height,
            std::max(capabilities.minImageExtent.height, extent.height));

        return extent;
    }
}

uint32_t
chooseImageCount(const vk::PresentModeKHR& present_mode,
                 const vk::SurfaceCapabilitiesKHR& capabilities)
{
    uint32_t res = 2;
    if (present_mode == vk::PresentModeKHR::eMailbox) res = 3;
    return std::min(capabilities.maxImageCount, res);
}

} // namespace

SwapChain::SwapChain(vk::Device logical_device,
                     const PhysicalDevice& physical_device,
                     vk::SurfaceKHR surface,
                     vk::Format depth_format,
                     vk::SurfaceFormatKHR surface_format,
                     uint32_t width,
                     uint32_t height)
{
    setupSwapchain(logical_device, physical_device, surface, surface_format,
                   width, height);
}

void
SwapChain::setupSwapchain(vk::Device logical_device,
                          const PhysicalDevice& physical_device,
                          vk::SurfaceKHR surface,
                          vk::SurfaceFormatKHR surface_format,
                          uint32_t width,
                          uint32_t height)
{
    auto support = physical_device.getSurfaceSupportDetails(surface);

    vk::PresentModeKHR present_mode = choosePresentMode(support.present_modes);

    m_extent = chooseExtent(width, height, support.capabilities);

    uint32_t image_count = chooseImageCount(present_mode, support.capabilities);

    vk::SwapchainCreateInfoKHR create_info = vk::SwapchainCreateInfoKHR(
        vk::SwapchainCreateFlagsKHR(), surface, image_count,
        surface_format.format, surface_format.colorSpace, m_extent, 1,
        vk::ImageUsageFlagBits::eColorAttachment);

    QueueFamilyIndices indices = physical_device.getQueueFamilyIndices();

    uint32_t queue_family_indices[] = {indices.graphics_family.value(),
                                       indices.present_family.value()};

    if (indices.graphics_family != indices.present_family)
    {
        create_info.imageSharingMode      = vk::SharingMode::eConcurrent;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices   = queue_family_indices;
    }
    else
    {
        create_info.imageSharingMode = vk::SharingMode::eExclusive;
    }

    create_info.preTransform   = support.capabilities.currentTransform;
    create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    create_info.presentMode    = present_mode;
    create_info.clipped        = vk::True;

    create_info.oldSwapchain = vk::SwapchainKHR(nullptr);

    auto&& res = logical_device.createSwapchainKHRUnique(create_info);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create swapchain");
    }
    m_swapchain = std::move(res.value);
}

const vk::Extent2D&
SwapChain::getExtent() const
{
    return m_extent;
}

void
SwapChain::recreate(vk::Device logical_device)
{
    logical_device.waitIdle();
    destroy();
}

SwapChain::
operator vk::SwapchainKHR() const
{
    return m_swapchain.get();
}

} // namespace ars_graphics
