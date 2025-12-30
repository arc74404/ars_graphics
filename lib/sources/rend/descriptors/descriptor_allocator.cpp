#include "descriptor_allocator.hpp"

#include "../device/logical_device.hpp"

namespace ars_graphics
{
DescriptorAllocator::DescriptorAllocator(
    const LogicalDevice& device,
    const std::vector<DescriptorBindingData>& bindings,
    uint32_t max_sets,
    uint32_t desc_count)
    : m_layout(device, bindings), m_pool(device, bindings, max_sets, desc_count)
{
}

const vk::DescriptorSetLayout&
DescriptorAllocator::layout() const
{
    return m_layout.descriptorSetLayout();
}

bool
DescriptorAllocator::allocate(const LogicalDevice& device,
                              vk::UniqueDescriptorSet& set) const
{
    vk::DescriptorSetAllocateInfo alloc_info(m_pool.descriptorPool(), 1,
                                             &m_layout.descriptorSetLayout());

    auto&& res = device.get().allocateDescriptorSetsUnique(alloc_info);

    if (res.result != vk::Result::eSuccess)
    {
        return false;
    }

    set = std::move(res.value[0]);

    return true;
}
} // namespace ars_graphics
