#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <string_view>

#include "validation_layers.hpp"

namespace ars_graphics
{
class Instance final
{
public:
    ~Instance();

    Instance() = default;

    void create(std::string_view app_name);

    std::vector<vk::PhysicalDevice> getAvailablePhysicalDevices() const;

    const vk::Instance& get() const;

private:
    std::vector<const char*> getExtensions();

    void createInstance(std::string_view app_name);

    void createDebugMessenger();

    vk::UniqueInstance m_instance;

    ValidationLayers m_validation_layers;

    vk::detail::DynamicLoader dl;
    vk::detail::DispatchLoaderDynamic dldi;

    vk::DebugUtilsMessengerEXT m_debug_messenger;
};
} // namespace ars_graphics
