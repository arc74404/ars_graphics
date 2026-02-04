#pragma once
#include <vector>

#include "../descriptors/descriptor_allocator.hpp"
#include "../device/logical_device.hpp"

namespace ars_graphics
{

struct DescripterWriteInfo
{
    vk::DescriptorBufferInfo buffer_info;
    uint32_t dst_binding;
    vk::DescriptorType descriptor_type;
};

class DescriptorWriter
{
public:
    void setup(const std::vector<DescripterWriteInfo>& descriptor_writes_info,
               const vk::DescriptorSet& dset);

    void write(const LogicalDevice& device);

private:
    std::vector<vk::WriteDescriptorSet> m_descriptor_writes;
};
} // namespace ars_graphics
