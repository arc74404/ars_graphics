#include "descriptor_pool.hpp"

#include <iostream>

#include "../device/logical_device.hpp"

namespace ars_graphics
{

const vk::DescriptorPool&
DescriptorPool::descriptorPool() const
{
    return m_descriptor_pool.get();
}

DescriptorPool::DescriptorPool(
    const LogicalDevice& device,
    const std::vector<DescriptorBindingData>& descriptor_set_layout_data,
    uint32_t max_sets,
    uint32_t desc_count)
{
    std::vector<vk::DescriptorPoolSize> poolSizes;

    for (int i = 0; i < descriptor_set_layout_data.size(); i++)
    {
        vk::DescriptorPoolSize poolSize;
        poolSize.type            = descriptor_set_layout_data[i].type;
        poolSize.descriptorCount = desc_count;
        poolSizes.push_back(poolSize);
    }

    vk::DescriptorPoolCreateInfo poolInfo;

    poolInfo.flags   = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
    poolInfo.maxSets = max_sets;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes    = poolSizes.data();

    auto&& res = device.get().createDescriptorPoolUnique(poolInfo);

    if (res.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create descriptor pool");
    }

    m_descriptor_pool = std::move(res.value);
}
} // namespace ars_graphics
