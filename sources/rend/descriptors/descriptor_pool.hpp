#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "descriptor_set_layout.hpp"

namespace ars_graphics
{
class DescriptorPool
{
public:
    DescriptorPool(
        const LogicalDevice& device,
        const std::vector<DescriptorBindingData>& descriptor_set_layout_data,
        uint32_t max_sets,
        uint32_t desc_count);

    const vk::DescriptorPool& descriptorPool() const;

private:
    vk::UniqueDescriptorPool m_descriptor_pool;
};
}; // namespace ars_graphics
