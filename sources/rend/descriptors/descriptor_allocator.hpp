#pragma once

#include "descriptor_pool.hpp"
#include "descriptor_set_layout.hpp"

namespace ars_graphics
{
class DescriptorAllocator final
{
public:
    DescriptorAllocator(const LogicalDevice& device,
                        const std::vector<DescriptorBindingData>& bindings,
                        uint32_t max_sets,
                        uint32_t desc_count);

    const vk::DescriptorSetLayout& layout() const;

    std::optional<vk::UniqueDescriptorSet> allocate(
        const LogicalDevice& device,
        vk::UniqueDescriptorSet& set) const;

private:
    DescriptorSetLayout m_layout;
    DescriptorPool m_pool;
};
} // namespace ars_graphics
