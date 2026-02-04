#include "descriptor_writer.hpp"

namespace ars_graphics
{

void
DescriptorWriter::setup(
    const std::vector<DescripterWriteInfo>& descriptor_writes_info,
    const vk::DescriptorSet& dset)
{
    m_descriptor_writes.resize(descriptor_writes_info.size());

    for (uint32_t i = 0; i < descriptor_writes_info.size(); ++i)
    {
        m_descriptor_writes[i].dstSet = dset;
        m_descriptor_writes[i].dstBinding =
            descriptor_writes_info[i].dst_binding;
        m_descriptor_writes[i].dstArrayElement = 0;
        m_descriptor_writes[i].descriptorType =
            descriptor_writes_info[i].descriptor_type;
        m_descriptor_writes[i].descriptorCount = 1;
        m_descriptor_writes[i].pBufferInfo =
            &descriptor_writes_info[i].buffer_info;
    }
}

void
DescriptorWriter::write(const LogicalDevice& device)
{
    device.get().updateDescriptorSets(m_descriptor_writes.size(),
                                      m_descriptor_writes.data(), 0, nullptr);
}
} // namespace ars_graphics
