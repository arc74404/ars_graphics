#pragma once

#include <glm/mat4x4.hpp>

#include <my_render.hpp>

namespace ars_graphics
{
class ResourceScene3D : public IResourceScene<ResourceScene>
{
public:
    void setFrameBuffersCount(size_t count)
    {
        m_uniform_buffers.resize(count);
        m_model_inst_storage_buffers.resize(count);
        m_mesh_inst_storage_buffers.resize(count);
        m_mapping_inst_storage_buffers.resize(count);
    }

    void updateCamera(glm::mat4 camera_matrix, uint32_t current_frame_index)
    {
        m_uniform_buffers[current_frame_index].setData(&camera_matrix,
                                                       sizeof(camera_matrix));
    }

private:
    vk::DescriptorSetLayout m_layout;

    std::vector<UniformBuffer> m_uniform_buffers;
    std::vector<StorageBuffer> m_model_inst_storage_buffers;
    std::vector<StorageBuffer> m_mesh_inst_storage_buffers;
    std::vector<StorageBuffer> m_mapping_inst_storage_buffers;

    std::vector<vk::UniqueDescriptorSet> m_descriptors;
};
} // namespace ars_graphics
