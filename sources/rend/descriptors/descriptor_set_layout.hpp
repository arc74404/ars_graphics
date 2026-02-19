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

class DescriptorSetLayout final
{
public:
    DescriptorSetLayout(const LogicalDevice& device,
                        const std::vector<DescriptorBindingData>& data);

    const vk::DescriptorSetLayout& get() const;

private:
    vk::UniqueDescriptorSetLayout m_descriptor_set_layout;
};
}; // namespace ars_graphics
