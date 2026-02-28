#pragma once

#include <vector>

#include "../device/logical_device.hpp"
#include "../libs_includes/vulkan.hpp"

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
