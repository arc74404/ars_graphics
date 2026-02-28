#pragma once
#include <vector>

#include "../device/logical_device.hpp"

#include "descriptor_allocator.hpp"

namespace ars_graphics
{

struct DescripterBufferWriteInfo
{
    vk::DescriptorBufferInfo buffer_info;
    uint32_t dst_binding;
    vk::DescriptorType descriptor_type;
};

struct DescripterTextureWriteInfo
{
    vk::DescriptorImageInfo texture_info;
    uint32_t dst_binding;
    vk::DescriptorType descriptor_type;
};

using DescriptorWrites = std::vector<vk::WriteDescriptorSet>;

DescriptorWrites
createBufferDescriptorWrites(
    const std::vector<DescripterBufferWriteInfo>& descriptor_writes_info,
    vk::DescriptorSet dset);

DescriptorWrites
createTextureDescriptorWrites(
    const std::vector<DescripterTextureWriteInfo>& descriptor_writes_info,
    vk::DescriptorSet dset);

} // namespace ars_graphics
