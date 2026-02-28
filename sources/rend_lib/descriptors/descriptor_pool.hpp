#pragma once

#include "../libs_includes/vulkan.hpp"

#include "descriptor_set_layout.hpp"

namespace ars_graphics
{

std::optional<vk::UniqueDescriptorPool>
createDescriptorPool(
    vk::Device device,
    const std::vector<DescriptorBindingData>& descriptor_set_layout_data,
    uint32_t max_sets,
    uint32_t desc_count);

}; // namespace ars_graphics
