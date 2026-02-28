#include "descriptor_set_layout.hpp"

#include <iostream>

#include "../device/logical_device.hpp"

std::optional<vk::UniqueDescriptorSetLayout>
ars_graphics::createDescriptorSetLayout(
    vk::Device device,
    const std::vector<DescriptorBindingData>& bindings_data)
{
    std::vector<vk::DescriptorSetLayoutBinding> layout_bindings;
    layout_bindings.reserve(bindings_data.size());

    for (auto&& bind_d : bindings_data)
    {
        layout_bindings.emplace_back(bind_d.binding_index, bind_d.type,
                                     bind_d.count, bind_d.stage, nullptr);
    }

    auto&& res = device.createDescriptorSetLayoutUnique(
        vk::DescriptorSetLayoutCreateInfo(
            vk::DescriptorSetLayoutCreateFlagBits(), bindings_data.size(),
            layout_bindings.data()));

    if (res.result != vk::Result::eSuccess)
    {
        return std::nullopt;
    }

    return std::move(res.value);
}
