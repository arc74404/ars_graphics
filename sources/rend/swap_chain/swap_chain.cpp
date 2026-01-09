#include "swap_chain.hpp"

#include "../render_pass/render_pass_creater.hpp"

namespace ars_graphics
{

namespace
{
vk::SurfaceFormatKHR
chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats)
{
    for (vk::SurfaceFormatKHR format : available_formats)
    {
        if (format.format == vk::Format::eB8G8R8A8Unorm &&
            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return format;
        }
    }

    return available_formats[0];
}

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

SwapChain::SwapChain(const LogicalDevice& logical_device,
                     const PhysicalDevice& physical_device,
                     const vk::SurfaceKHR& surface,
                     uint32_t width,
                     uint32_t height)
    : m_depth_format(physical_device.calculateSwapchainDepthFormat()),
      m_surface_format(chooseSurfaceFormat(
          physical_device.calculateSurfaceFormats(surface))),
      m_renderpass_manager(logical_device,
                           m_surface_format.format,
                           m_depth_format)
{
    setupSwapchain(logical_device, physical_device, surface, width, height);

    auto&& images =
        logical_device.get().getSwapchainImagesKHR(m_swapchain.get());

    if (false == images.has_value())
    {
        throw std::runtime_error("Failed getSwapchainImagesKHR in swapchain");
    }

    for (size_t i = 0; i < images.value.size(); ++i)
    {
        m_frames.emplace_back(logical_device, physical_device, images.value[i],
                              m_surface_format.format, m_depth_format, m_extent,
                              m_renderpass_manager);
    }
}

void
SwapChain::setupSwapchain(const LogicalDevice& logical_device,
                          const PhysicalDevice& physical_device,
                          const vk::SurfaceKHR& surface,
                          uint32_t width,
                          uint32_t height)
{
    auto support = physical_device.getSurfaceSupportDetails(surface);

    vk::PresentModeKHR present_mode = choosePresentMode(support.present_modes);

    m_extent = chooseExtent(width, height, support.capabilities);

    uint32_t image_count = chooseImageCount(present_mode, support.capabilities);

    vk::SwapchainCreateInfoKHR create_info = vk::SwapchainCreateInfoKHR(
        vk::SwapchainCreateFlagsKHR(), surface, image_count,
        m_surface_format.format, m_surface_format.colorSpace, m_extent, 1,
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

    auto&& res = logical_device.get().createSwapchainKHRUnique(create_info);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create swapchain");
    }
    m_swapchain = std::move(res.value);
}

size_t
SwapChain::countFrames() const
{
    return m_frames.size();
}

void
SwapChain::destroy()
{
    m_swapchain.reset();
}

} // namespace ars_graphics
