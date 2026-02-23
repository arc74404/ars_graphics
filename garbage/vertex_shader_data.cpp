#include "vertex_shader_data.hpp"

#include "../global_settings.hpp"

namespace ars_graphics
{

namespace
{
DescripterWriteInfo
createDescripterWriteInfo(const vk::Buffer& buffer,
                          size_t range,
                          size_t binding,
                          vk::DescriptorType type)
{
    DescripterWriteInfo res;
    res.dst_binding        = binding;
    res.descriptor_type    = type;
    res.buffer_info.buffer = buffer;
    res.buffer_info.offset = 0;
    res.buffer_info.range  = range;
    return res;
}
} // namespace

VertexShaderData::VertexShaderData(
    const ars_graphics::LogicalDevice& logical_device,
    const ars_graphics::PhysicalDevice& physical_device,
    uint32_t count_frames,
    const DescriptorAllocator& descriptor_allocator)
    : m_uniform_buffers(count_frames),
      m_model_inst_storage_buffers(count_frames),
      m_mesh_inst_storage_buffers(count_frames),
      m_mapping_inst_storage_buffers(count_frames),
      m_descriptors(count_frames)
{
    for (auto&& descriptor : m_descriptors)
    {
        descriptor_allocator.allocate(logical_device, descriptor);
    }
}

void
VertexShaderData::updateUbo(const UboData& ubo)
{
    m_ubo = ubo;
}

void
VertexShaderData::bind(const vk::CommandBuffer& buffer,
                       vk::PipelineLayout pipeline_layout,
                       uint32_t frame_index)
{
    buffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline_layout,
                              0, 1, &(m_descriptors[frame_index].get()), 0,
                              nullptr);
}

void
VertexShaderData::init(const ars_graphics::LogicalDevice& logical_device,
                       const ars_graphics::PhysicalDevice& physical_device,
                       const PerFrameLinksOnData& links)
{
    if (false == (links.m_mapping && links.m_mesh_instancing_data &&
                  links.m_model_instancing_data))
    {
        throw std::logic_error("Instance Data is nullptr");
    }
    m_links = links;

    for (int i = 0; i < m_uniform_buffers.size(); ++i)
    {
        updatePerFrameData(logical_device, physical_device, i);
        updateDescriptorSets(logical_device, i);
    }
}

void
VertexShaderData::updateDescriptorSets(
    const ars_graphics::LogicalDevice& logical_device,
    uint32_t current_frame_index)
{
    std::vector<DescripterWriteInfo> descriptor_write_info(4);
    descriptor_write_info[0] = createDescripterWriteInfo(
        m_uniform_buffers[current_frame_index].buffer(), sizeof(UboData),
        settings::bindings::uniform_buffer_binding,
        vk::DescriptorType::eUniformBuffer);

    descriptor_write_info[1] = createDescripterWriteInfo(
        m_model_inst_storage_buffers[current_frame_index].buffer(),
        m_links.m_model_instancing_data->size() * sizeof(ModelInstancingData),
        settings::bindings::instancing_binding,
        vk::DescriptorType::eStorageBuffer);

    descriptor_write_info[2] = createDescripterWriteInfo(
        m_mesh_inst_storage_buffers[current_frame_index].buffer(),
        m_links.m_mesh_instancing_data->size() * sizeof(MeshInstancingData),
        settings::bindings::instancing_binding + 1,
        vk::DescriptorType::eStorageBuffer);

    descriptor_write_info[3] = createDescripterWriteInfo(
        m_mapping_inst_storage_buffers[current_frame_index].buffer(),
        m_links.m_mapping->size() * sizeof(InstanceMapping),
        settings::bindings::instancing_binding + 2,
        vk::DescriptorType::eStorageBuffer);

    m_descriptor_writer.setup(descriptor_write_info,
                              m_descriptors[current_frame_index].get());
    m_descriptor_writer.write(logical_device);
}

void
VertexShaderData::updatePerFrameData(
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
        logical_device, physical_device, &m_ubo, sizeof(UboData));

    m_model_inst_storage_buffers[current_frame_index].setData(
        logical_device, physical_device,
        m_links.m_model_instancing_data->data(),
        m_links.m_model_instancing_data->size() * sizeof(ModelInstancingData));

    m_mesh_inst_storage_buffers[current_frame_index].setData(
        logical_device, physical_device, m_links.m_mesh_instancing_data->data(),
        m_links.m_mesh_instancing_data->size() * sizeof(MeshInstancingData));

    m_mapping_inst_storage_buffers[current_frame_index].setData(
        logical_device, physical_device, m_links.m_mapping->data(),
        m_links.m_mapping->size() * sizeof(InstanceMapping));
}
} // namespace ars_graphics
