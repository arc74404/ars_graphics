#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <string>
#include <unordered_map>

#include "descriptor_allocator.hpp"

namespace ars_graphics
{

enum class DescriptorSetLayoutType
{
    MODEL_VERTEX,
    MODEL_MATERIAL
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
