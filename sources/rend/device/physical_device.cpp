#include "physical_device.hpp"

#include <iostream>
#include <set>
#include <string>

namespace ars_graphics
{
bool
PhysicalDevice::checkDeviceExtensionSupport(
    const vk::PhysicalDevice& device,
    const std::vector<const char*>& requested_extensions)
{
    std::set<std::string> requiredExtensions(requested_extensions.begin(),
                                             requested_extensions.end());

    auto&& props = device.enumerateDeviceExtensionProperties();

    if (props.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed enumerate extension properties");
    }

    for (vk::ExtensionProperties& extension : props.value)
    {
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}

bool
PhysicalDevice::supportMailBox(const vk::PhysicalDevice& device,
                               const vk::SurfaceKHR& surface)
{
    auto&& modes = device.getSurfacePresentModesKHR(surface);
    if (modes.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed get surface present modes");
    }
    for (vk::PresentModeKHR presentMode : modes.value)
    {
        if (presentMode == vk::PresentModeKHR::eMailbox)
        {
            return true;
        }
    }
    return false;
}

bool
PhysicalDevice::isSuitable(const vk::PhysicalDevice& device)
{
    static std::vector<const char*> requested_extensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    return checkDeviceExtensionSupport(device, requested_extensions);
}

PhysicalDevice::PhysicalDevice(const Instance& instance,
                               const vk::SurfaceKHR& surface)
{
    choosePhysicalDevice(instance, surface);

    m_queue_family_indices = findQueueFamilies(surface);
}

QueueFamilyIndices
PhysicalDevice::getQueueFamilyIndices() const
{
    return m_queue_family_indices;
}

void
PhysicalDevice::choosePhysicalDevice(const Instance& instance,
                                     const vk::SurfaceKHR& surface)
{
    auto available_devices = instance.getAvailablePhysicalDevices();

    for (vk::PhysicalDevice device : available_devices)
    {
        if (isSuitable(device) && supportMailBox(device, surface))
        {
            m_physical_device = device;
            return;
        }
    }
    for (vk::PhysicalDevice device : available_devices)
    {
        if (isSuitable(device))
        {
            m_physical_device = device;
            return;
        }
    }
    throw std::exception("Could not choose phsical device\n");
}

bool
QueueFamilyIndices::isComplete()
{
    return graphics_family.has_value() && present_family.has_value();
}

QueueFamilyIndices
PhysicalDevice::findQueueFamilies(const vk::SurfaceKHR& surface) const
{
    QueueFamilyIndices indices;

    std::vector<vk::QueueFamilyProperties> queueFamilies =
        m_physical_device.getQueueFamilyProperties();

    int i = 0;

    for (vk::QueueFamilyProperties queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            indices.graphics_family = i;
        }
        auto&& res = m_physical_device.getSurfaceSupportKHR(i, surface);

        if (res.result != vk::Result::eSuccess)
        {
            throw std::runtime_error("Failed getSurfaceSupportKHR");
        }

        if (res.value)
        {
            indices.present_family = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

uint32_t
PhysicalDevice::findMemoryTypeIndex(
    uint32_t supported_memory_indices,
    vk::MemoryPropertyFlags requested_properties) const
{
    vk::PhysicalDeviceMemoryProperties memoryProperties =
        m_physical_device.getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        bool supported{static_cast<bool>(supported_memory_indices & (1 << i))};

        bool sufficient{(memoryProperties.memoryTypes[i].propertyFlags &
                         requested_properties) == requested_properties};

        if (supported && sufficient)
        {
            return i;
        }
    }

    return 0;
}

std::vector<vk::SurfaceFormatKHR>
PhysicalDevice::calculateSurfaceFormats(const vk::SurfaceKHR& surface) const
{
    auto&& forms = m_physical_device.getSurfaceFormatsKHR(surface);

    if (forms.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed getSurfaceSupportDetails");
    }

    return forms.value;
}

SwapChainSupportDetails
PhysicalDevice::getSurfaceSupportDetails(const vk::SurfaceKHR& surface) const
{
    auto&& cap = m_physical_device.getSurfaceCapabilitiesKHR(surface);

    auto&& modes = m_physical_device.getSurfacePresentModesKHR(surface);

    if (cap.result != vk::Result::eSuccess ||
        modes.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed getSurfaceSupportDetails");
    }

    return {.capabilities = cap.value, .present_modes = modes.value};
}

vk::Format
PhysicalDevice::calculateSwapchainDepthFormat() const
{
    const std::vector<vk::Format> candidates = {
        vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint,
        vk::Format::eD32Sfloat, vk::Format::eD16UnormS8Uint,
        vk::Format::eD16Unorm};

    for (auto&& format : candidates)
    {
        vk::FormatProperties props =
            m_physical_device.getFormatProperties(format);

        if (props.optimalTilingFeatures &
            vk::FormatFeatureFlagBits::eDepthStencilAttachment)
        {
            return format;
        }
    }

    throw std::runtime_error("Failed to find supported depth format!");
}

const vk::PhysicalDevice&
PhysicalDevice::get() const
{
    return m_physical_device;
}
} // namespace ars_graphics
