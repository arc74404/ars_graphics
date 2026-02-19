#include "descriptor_set_layout.hpp"

#include <iostream>

#include "../device/logical_device.hpp"

namespace ars_graphics
{
DescriptorSetLayout::DescriptorSetLayout(
    const LogicalDevice& device,
    const std::vector<DescriptorBindingData>& data)
{
    std::vector<vk::DescriptorSetLayoutBinding> layout_bindings;
    layout_bindings.reserve(data.size());

    for (int i = 0; i < data.size(); i++)
    {
        vk::DescriptorSetLayoutBinding layout_binding;
        layout_binding.binding            = data[i].binding_index;
        layout_binding.descriptorType     = data[i].type;
        layout_binding.descriptorCount    = data[i].count;
        layout_binding.stageFlags         = data[i].stage;
        layout_binding.pImmutableSamplers = nullptr;
        layout_bindings.emplace_back(layout_binding);
    }

    vk::DescriptorSetLayoutCreateInfo layout_info;
    layout_info.flags        = vk::DescriptorSetLayoutCreateFlagBits();
    layout_info.bindingCount = data.size();
    layout_info.pBindings    = layout_bindings.data();

    auto&& res = device.get().createDescriptorSetLayoutUnique(layout_info);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create descriptor set layout");
    }

    m_descriptor_set_layout = std::move(res.value);
}

const vk::DescriptorSetLayout&
DescriptorSetLayout::get() const
{
    return m_descriptor_set_layout.get();
}
}; // namespace ars_graphics
