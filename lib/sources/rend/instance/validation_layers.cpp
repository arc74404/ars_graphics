#include "validation_layers.hpp"

namespace ars_graphics
{
void
ValidationLayers::create()
{
    m_layers.push_back("VK_LAYER_KHRONOS_validation");
}

uint32_t
ValidationLayers::count()
{
    return m_layers.size();
}

const char**
ValidationLayers::data()
{
    return m_layers.data();
}

const char*
ValidationLayers::getSupportObjectNameImpl(
    const vk::ExtensionProperties& extension_prop)
{
    return extension_prop.extensionName;
}

const char*
ValidationLayers::getSupportObjectNameImpl(
    const vk::LayerProperties& layer_prop)
{
    return layer_prop.layerName;
}

bool
ValidationLayers::supported(std::vector<const char*> extensions)
{
    auto&& ext_prop   = vk::enumerateInstanceExtensionProperties();
    auto&& layer_prop = vk::enumerateInstanceLayerProperties();

    if (ext_prop.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed enumerateInstanceExtensionProperties");
    }
    if (layer_prop.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed enumerateInstanceLayerProperties");
    }

    return checkSupport<vk::ExtensionProperties>(extensions, ext_prop.value) &&
           checkSupport<vk::LayerProperties>(m_layers, layer_prop.value);
}
} // namespace ars_graphics
