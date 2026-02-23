#include "descriptor_writer.hpp"

ars_graphics::DescriptorWrites
ars_graphics::createBufferDescriptorWrites(
    const std::vector<DescripterBufferWriteInfo>& descriptor_writes_info,
    vk::DescriptorSet dset)
{
    DescriptorWrites res;
    res.reserve(descriptor_writes_info.size());

    for (auto&& d : descriptor_writes_info)
    {
        res.emplace_back(dset, d.dst_binding, 0, 1, d.descriptor_type, nullptr,
                         &d.buffer_info);
    }
    return res;
}

ars_graphics::DescriptorWrites
ars_graphics::createTextureDescriptorWrites(
    const std::vector<DescripterTextureWriteInfo>& descriptor_writes_info,
    vk::DescriptorSet dset)
{
    DescriptorWrites res;
    res.reserve(descriptor_writes_info.size());

    for (auto&& d : descriptor_writes_info)
    {
        res.emplace_back(dset, d.dst_binding, 0, 1, d.descriptor_type,
                         &d.texture_info);
    }
    return res;
}
