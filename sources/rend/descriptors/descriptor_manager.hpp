#pragma once

#include <string>
#include <unordered_map>

#include "../../libs_includes/vulkan.hpp"

#include "descriptor_allocator.hpp"

namespace ars_graphics
{
template <typename KeyT>
class DescriptorManager final
{
public:
    DescriptorManager(const LogicalDevice& device) : m_device(device)
    {
    }

    bool addAllocator(const T& allocator_key,
                      const LogicalDevice& device,
                      const std::vector<DescriptorBindingData>& bindings,
                      uint32_t max_sets,
                      uint32_t desc_count)
    {
        m_descriptor_allocator_storage.emplace(allocator_key, device, bindings,
                                               max_sets, desc_count);
    }

    std::optional<const DescriptorAllocator&> getAllocator(
        const T& allocator_key) const
    {
        auto&& find_it = m_descriptor_allocator_storage.find(allocator_key);

        if (find_it == m_descriptor_allocator_storage.end())
        {
            return std::nullopt;
        }
        return find_it->second;
    }

private:
    void addAllocatorImpl(const LogicalDevice& device,
                          const std::vector<DescriptorBindingData>& bindings,
                          uint32_t max_sets,
                          uint32_t desc_count);

    std::unordered_map<KeyT, DescriptorAllocator>
        m_descriptor_allocator_storage;

    const LogicalDevice& m_device;
};

}; // namespace ars_graphics
