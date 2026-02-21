#include "descriptor_allocator.hpp"

#include "../device/logical_device.hpp"

namespace ars_graphics
{
DescriptorAllocator::DescriptorAllocator(
    vk::Device device,
    const std::vector<DescriptorBindingData>& bindings,
    uint32_t max_sets,
    uint32_t desc_count)
{
    auto&& layout = createDescriptorSetLayout(device, bindings);
    auto&& pool = createDescriptorPool(device, bindings, max_sets, desc_count);

    if (false == (layout.has_value() && pool.has_value()))
    {
        throw std::runtime_error("failed to create DescriptorAllocator");
    }
    m_layout = std::move(layout.value());
    m_pool   = std::move(pool.value());
}

std::optional<vk::UniqueDescriptorSet>
DescriptorAllocator::allocate(vk::Device device) const
{
    vk::DescriptorSetAllocateInfo alloc_info(m_pool.get(), 1, &m_layout.get());

    auto&& res = device.allocateDescriptorSetsUnique(alloc_info);

    if (res.result != vk::Result::eSuccess)
    {
        return std::nullopt;
    }

    return std::move(res.value[0]);
}
} // namespace ars_graphics
