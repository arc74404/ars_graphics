#include "resource_scene_3d.hpp"

#include "instance_data.hpp"

namespace ars_graphics
{

ResourceScene3D::ResourceScene3D(const RenderSetuper& setuper)
    : IResourceScene<ResourceScene3D>(setuper)
{
    setFrameBuffersCount(m_setuper.getFramesCount());

    std::vector<DescriptorBindingData> bindings_data;

    bindings_data.emplace_back(
        DescriptorBindingData(9, vk::DescriptorType::eUniformBuffer, 1,
                              vk::ShaderStageFlags::BitsType::eVertex));
    bindings_data.emplace_back(
        DescriptorBindingData(10, vk::DescriptorType::eStorageBuffer, 1,
                              vk::ShaderStageFlags::BitsType::eVertex));
    bindings_data.emplace_back(
        DescriptorBindingData(11, vk::DescriptorType::eStorageBuffer, 1,
                              vk::ShaderStageFlags::BitsType::eVertex));
    bindings_data.emplace_back(
        DescriptorBindingData(12, vk::DescriptorType::eStorageBuffer, 1,
                              vk::ShaderStageFlags::BitsType::eVertex));

    m_alloc =
        m_setuper.createDescriptorAllocator(std::move(bindings_data), 100, 100);

    m_descriptors = m_alloc.allocate(m_setuper.getFramesCount()).value();

    for (uint32_t i = 0; i < m_uniform_buffers.size(); ++i)
    {
        updateDescriptorSets(i);
    }
}

void
ResourceScene3D::bindDescriptors(vk::CommandBuffer cmd,
                                 vk::PipelineLayout pipeline_layout,
                                 uint32_t frame_index) const
{
    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline_layout, 0,
                           1, &m_descriptors[frame_index].get(), 0, nullptr);
}

void
ResourceScene3D::updateDescriptorSets(uint32_t frame_index)
{
    std::vector<vk::DescriptorBufferInfo> buffer_infos;
    std::vector<vk::WriteDescriptorSet> writes;

    vk::DescriptorBufferInfo uniform_info{m_uniform_buffers[frame_index], 0,
                                          sizeof(glm::mat4)};
    writes.emplace_back(vk::WriteDescriptorSet{
        m_descriptors[frame_index].get(), 9, 0, 1,
        vk::DescriptorType::eUniformBuffer, nullptr, &uniform_info, nullptr});

    vk::DescriptorBufferInfo model_info{
        m_model_inst_storage_buffers[frame_index], 0,
        m_model_inst_storage_buffers[frame_index].getByteSize()};

    writes.emplace_back(vk::WriteDescriptorSet{
        m_descriptors[frame_index].get(), 10, 0, 1,
        vk::DescriptorType::eStorageBuffer, nullptr, &model_info, nullptr});

    vk::DescriptorBufferInfo mesh_info{
        m_mesh_inst_storage_buffers[frame_index], 0,
        m_mesh_inst_storage_buffers[frame_index].getByteSize()};

    writes.emplace_back(vk::WriteDescriptorSet{
        m_descriptors[frame_index].get(), 11, 0, 1,
        vk::DescriptorType::eStorageBuffer, nullptr, &mesh_info, nullptr});

    vk::DescriptorBufferInfo mapping_info{
        m_mapping_inst_storage_buffers[0], 0,
        m_mapping_inst_storage_buffers[0].getByteSize()};

    writes.emplace_back(vk::WriteDescriptorSet{
        m_descriptors[frame_index].get(), 12, 0, 1,
        vk::DescriptorType::eStorageBuffer, nullptr, &mapping_info, nullptr});

    m_setuper.updateDescriptorSets(writes);
}

void
ResourceScene3D::setFrameBuffersCount(size_t count)
{
    m_uniform_buffers.resize(count);
    m_model_inst_storage_buffers.resize(count);
    m_mesh_inst_storage_buffers.resize(count);
    m_mapping_inst_storage_buffers.resize(1);
}

void
ResourceScene3D::updateCamera(glm::mat4 camera_matrix,
                              uint32_t current_frame_index)
{
    m_uniform_buffers[current_frame_index].setData(&camera_matrix,
                                                   sizeof(camera_matrix));
}

void
ResourceScene3D::updateModelInstancing(
    const std::vector<user_part::ModelInstancingData>& data,
    uint32_t frame_index)
{
    m_model_inst_storage_buffers[frame_index].setData(
        data.data(), data.size() * sizeof(user_part::ModelInstancingData));
}

void
ResourceScene3D::updateMeshInstancing(
    const std::vector<user_part::MeshInstancingData>& data,
    uint32_t frame_index)
{
    m_mesh_inst_storage_buffers[frame_index].setData(
        data.data(), data.size() * sizeof(user_part::MeshInstancingData));
}

void
ResourceScene3D::updateMapping(
    const std::vector<user_part::InstanceMapping>& data)
{
    for (auto&& mp : m_mapping_inst_storage_buffers)
    {
        mp.setData(data.data(),
                   data.size() * sizeof(user_part::InstanceMapping));
    }
}

} // namespace ars_graphics
