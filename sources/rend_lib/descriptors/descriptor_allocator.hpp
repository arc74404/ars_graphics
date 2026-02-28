#pragma once

#include "descriptor_pool.hpp"
#include "descriptor_set_layout.hpp"

namespace ars_graphics
{
class DescriptorAllocator final
{
public:
    DescriptorAllocator(vk::Device device,
                        const std::vector<DescriptorBindingData>& bindings,
                        uint32_t max_sets,
                        uint32_t desc_count);

    std::optional<vk::UniqueDescriptorSet> allocate(vk::Device device) const;

private:
    vk::UniqueDescriptorSetLayout m_layout;
    vk::UniqueDescriptorPool m_pool;
};
} // namespace ars_graphics
