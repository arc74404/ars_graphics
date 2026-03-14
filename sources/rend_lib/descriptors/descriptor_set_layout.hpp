#pragma once

#include <vector>

#include "../device/logical_device.hpp"
#include "../libs_includes/vulkan.hpp"

namespace ars_graphics
{
struct DescriptorBindingData
{
    DescriptorBindingData(uint8_t bi,
                          vk::DescriptorType t,
                          uint8_t c,
                          vk::ShaderStageFlags s)
        : binding_index(bi), type(t), count(c), stage(s)
    {
    }
    uint8_t binding_index;
    vk::DescriptorType type;
    uint8_t count;
    vk::ShaderStageFlags stage;
};

std::optional<vk::UniqueDescriptorSetLayout>
createDescriptorSetLayoutImpl(
    vk::Device device,
    std::vector<DescriptorBindingData>&& bindings_data);

}; // namespace ars_graphics
