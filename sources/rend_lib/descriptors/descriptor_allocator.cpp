#include "descriptor_allocator.hpp"

#include "../device/logical_device.hpp"

namespace ars_graphics
{
DescriptorAllocator::DescriptorAllocator(
    vk::Device device,
    std::vector<DescriptorBindingData>&& bindings,
    uint32_t max_sets,
    uint32_t desc_count)
    : m_device(device)
{
    auto&& pool = createDescriptorPool(device, bindings, max_sets, desc_count);

    auto&& layout = createDescriptorSetLayoutImpl(device, std::move(bindings));

    if (false == (layout.has_value() && pool.has_value()))
    {
        throw std::runtime_error("failed to create DescriptorAllocator");
    }
    m_layout = std::move(layout.value());
    m_pool   = std::move(pool.value());
}

std::optional<std::vector<vk::UniqueDescriptorSet>>
DescriptorAllocator::allocate(size_t count) const
{
    vk::DescriptorSetAllocateInfo alloc_info(m_pool.get(), count,
                                             &m_layout.get());

    auto&& res = m_device.allocateDescriptorSetsUnique(alloc_info);

    if (res.result != vk::Result::eSuccess)
    {
        return std::nullopt;
    }

    return std::move(res.value);
}
} // namespace ars_graphics
