#pragma once

#include <vector>

#include "../../libs_includes/vulkan.hpp"
#include "../device/logical_device.hpp"

namespace ars_graphics
{
struct DescriptorBindingData
{
    uint8_t binding_index;
    vk::DescriptorType type;
    uint8_t count;
    vk::ShaderStageFlags stage;
};

std::optional<vk::UniqueDescriptorSetLayout>
createDescriptorSetLayout(
    vk::Device device,
    const std::vector<DescriptorBindingData>& bindings_data);

}; // namespace ars_graphics
