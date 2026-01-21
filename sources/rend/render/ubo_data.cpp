#include "ubo_data.hpp"

#include "../global_settings.hpp"

namespace ars_graphics
{

VertexShaderUbo::VertexShaderUbo(
    const ars_graphics::LogicalDevice& logical_device,
    const ars_graphics::PhysicalDevice& physical_device,
    uint32_t count_frames,
    const DescriptorAllocator& descriptor_allocator)
    : m_uniform_buffers(count_frames), m_descriptors(count_frames)
{
    for (auto&& descriptor : m_descriptors)
    {
        descriptor_allocator.allocate(logical_device, descriptor);
    }
    for (int i = 0; i < count_frames; ++i)
    {
        updatePerFrameUboBuffer(logical_device, physical_device, i);
        updateDescriptorSets(logical_device, i);
    }
}

void
VertexShaderUbo::bind(const vk::CommandBuffer& buffer,
                      vk::PipelineLayout pipeline_layout,
                      uint32_t frame_index)
{
    buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline_layout,
                              0, 1, &(m_descriptors[frame_index].get()), 0,
                              nullptr);
}

void
VertexShaderUbo::updateDescriptorSets(
    const ars_graphics::LogicalDevice& logical_device,
    uint32_t current_frame_index)
{
    vk::DescriptorBufferInfo buffer_info{};
    buffer_info.buffer = m_uniform_buffers[current_frame_index].buffer();
    buffer_info.offset = 0;
    buffer_info.range  = sizeof(UboData);

    vk::WriteDescriptorSet descriptor_write{};
    descriptor_write.dstSet     = m_descriptors[current_frame_index].get();
    descriptor_write.dstBinding = settings::bindings::uniform_buffer_binding;
    descriptor_write.dstArrayElement = 0;
    descriptor_write.descriptorType  = vk::DescriptorType::eUniformBuffer;
    descriptor_write.descriptorCount = 1;
    descriptor_write.pBufferInfo     = &buffer_info;

    logical_device.get().updateDescriptorSets(1, &descriptor_write, 0, nullptr);
}

void
VertexShaderUbo::VertexShaderUbo::updatePerFrameUboBuffer(
    const ars_graphics::LogicalDevice& logical_device,
    const ars_graphics::PhysicalDevice& physical_device,
    uint32_t current_frame_index)
{
    if (current_frame_index >= m_uniform_buffers.size())
    {
        throw std::runtime_error(
            "VertexShaderUbo not initialized or invalid frame index");
    }
    m_uniform_buffers[current_frame_index].setData(
        logical_device, physical_device, &m_ubo_data, sizeof(m_ubo_data));
}
} // namespace ars_graphics
