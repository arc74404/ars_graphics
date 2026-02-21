#include "descriptor_pool.hpp"

#include <iostream>
#include <numeric>

#include "../device/logical_device.hpp"

namespace
{
std::vector<vk::DescriptorPoolSize>
createPoolSizes(const std::vector<ars_graphics::DescriptorBindingData>&
                    descriptor_set_layout_data,
                uint32_t desc_count)
{
    std::vector<vk::DescriptorPoolSize> result;
    result.reserve(descriptor_set_layout_data.size());

    for (auto&& data : descriptor_set_layout_data)
    {
        result.emplace_back(data.type, desc_count);
    }
    return result;
}

vk::DescriptorPoolCreateInfo
createPoolInfo(uint32_t max_sets,
               uint32_t pool_sizes_count,
               vk::DescriptorPoolSize* p_data)
{
    return vk::DescriptorPoolCreateInfo(
        vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, max_sets,
        pool_sizes_count, p_data);
}

} // namespace

std::optional<vk::UniqueDescriptorPool>
ars_graphics::createDescriptorPool(
    vk::Device device,
    const std::vector<DescriptorBindingData>& descriptor_set_layout_data,
    uint32_t max_sets,
    uint32_t desc_count)
{
    std::vector<vk::DescriptorPoolSize> pool_sizes =
        createPoolSizes(descriptor_set_layout_data, desc_count);

    vk::DescriptorPoolCreateInfo pool_info =
        createPoolInfo(max_sets, pool_sizes.size(), pool_sizes.data());

    auto&& res = device.createDescriptorPoolUnique(pool_info);

    if (res.result != vk::Result::eSuccess)
    {
        return std::nullopt;
    }

    return std::move(res.value);
}
