#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <vector>

#include "../device/logical_device.hpp"

namespace ars_graphics
{
struct DescriptorBindingData
{
    int binding_index;
    vk::DescriptorType type;
    int count;
    vk::ShaderStageFlags stage;
};

class DescriptorSetLayout final
{
public:
    DescriptorSetLayout(const LogicalDevice& device,
                        const std::vector<DescriptorBindingData>& data);

    const vk::DescriptorSetLayout& descriptorSetLayout() const;

private:
    vk::UniqueDescriptorSetLayout m_descriptor_set_layout;
};
}; // namespace ars_graphics
