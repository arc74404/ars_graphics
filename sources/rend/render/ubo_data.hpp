#pragma once

#include <vector>

#include "../buffers/uniform_buffer.hpp"
#include "../descriptors/descriptor_allocator.hpp"
#include "glm/mat4x4.hpp"

namespace ars_graphics
{
struct UboData final
{
    void updateCamera(glm::mat4 new_camera_matrix)
    {
        camera_matrix = new_camera_matrix;
    }
    glm::mat4 camera_matrix;
};

class VertexShaderUbo final
{
public:
    VertexShaderUbo(const ars_graphics::LogicalDevice& logical_device,
                    const ars_graphics::PhysicalDevice& physical_device,
                    uint32_t count_frames,
                    const DescriptorAllocator& descriptor_allocator);

    void updatePerFrameUboBuffer(
        const ars_graphics::LogicalDevice& logical_device,
        const ars_graphics::PhysicalDevice& physical_device,
        uint32_t current_frame_index);

    void bind(const vk::CommandBuffer& buffer,
              vk::PipelineLayout pipeline_layout,
              uint32_t frame_index);

    void updateCamera(const glm::mat4& new_camera_matrix)
    {
        m_ubo_data.updateCamera(new_camera_matrix);
    }

private:
    void updateDescriptorSets(const ars_graphics::LogicalDevice& logical_device,
                              uint32_t current_frame_index);
    UboData m_ubo_data;

    vk::DescriptorSetLayout m_layout;

    std::vector<vk::UniqueDescriptorSet> m_descriptors;
    std::vector<UniformBuffer> m_uniform_buffers;
};
} // namespace ars_graphics
