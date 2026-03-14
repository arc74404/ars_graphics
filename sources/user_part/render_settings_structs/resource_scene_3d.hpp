#pragma once

#include <glm/mat4x4.hpp>

#include <my_render.hpp>

namespace ars_graphics
{
class ResourceScene3D : public IResourceScene<ResourceScene3D>
{
public:
    ResourceScene3D(const RenderSetuper& setuper);

    void bindDescriptors(vk::CommandBuffer cmd,
                         vk::PipelineLayout pipeline_layout,
                         uint32_t frame_index) const;

    void updateDescriptorSets(uint32_t frame_index);
    void setFrameBuffersCount(size_t count);
    void updateCamera(glm::mat4 camera_matrix, uint32_t current_frame_index);
    void updateModelInstancing(
        const std::vector<user_part::ModelInstancingData>& data,
        uint32_t frame_index);
    void updateMeshInstancing(
        const std::vector<user_part::MeshInstancingData>& data,
        uint32_t frame_index);
    void updateMapping(const std::vector<user_part::InstanceMapping>& data);

private:
    ars_graphics::DescriptorAllocator m_alloc;

    std::vector<UniformBuffer> m_uniform_buffers;
    std::vector<StorageBuffer> m_model_inst_storage_buffers;
    std::vector<StorageBuffer> m_mesh_inst_storage_buffers;
    std::vector<StorageBuffer> m_mapping_inst_storage_buffers;

    std::vector<vk::UniqueDescriptorSet> m_descriptors;
};
} // namespace ars_graphics
