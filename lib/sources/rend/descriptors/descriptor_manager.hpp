#ifndef DESCRIPTOR_MANAGER_HPP
#define DESCRIPTOR_MANAGER_HPP

#include <vulkan/vulkan.hpp>

#include <string>
#include <unordered_map>

#include "descriptor_allocator.hpp"

namespace ars_graphics
{

enum class DescriptorSetLayoutType
{
    UBO_x_STORAGE,
    COMBINED_IMAGE_SAMPLER
};

class DescriptorManager
{
public:
    DescriptorManager(const LogicalDevice& device);

    const DescriptorAllocator& getAllocator(
        DescriptorSetLayoutType sh_type) const;

private:
    void addAllocator(DescriptorSetLayoutType sh_type,
                      const LogicalDevice& device,
                      const std::vector<DescriptorBindingData>& bindings,
                      uint32_t max_sets,
                      uint32_t desc_count);

    std::unordered_map<DescriptorSetLayoutType, DescriptorAllocator>
        m_descriptor_allocator_storage;
};

}; // namespace ars_graphics

#endif // DESCRIPTOR_MANAGER_HPP
