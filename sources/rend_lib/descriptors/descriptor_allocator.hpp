#pragma once

#include "descriptor_pool.hpp"
#include "descriptor_set_layout.hpp"

namespace ars_graphics
{
class DescriptorAllocator final
{
public:
    DescriptorAllocator() = default;
    DescriptorAllocator(vk::Device device,
                        std::vector<DescriptorBindingData>&& bindings,
                        uint32_t max_sets,
                        uint32_t desc_count);

    std::optional<std::vector<vk::UniqueDescriptorSet>> allocate(
        size_t count) const;

private:
    vk::Device m_device;
    vk::UniqueDescriptorSetLayout m_layout;
    vk::UniqueDescriptorPool m_pool;
};
} // namespace ars_graphics
