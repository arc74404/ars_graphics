#pragma once

#include <vector>

#include "../buffers/storage_buffer.hpp"
#include "../buffers/uniform_buffer.hpp"
#include "../descriptors/descriptor_allocator.hpp"
#include "../shaders/shaders_data_structs/instance_data.hpp"
#include "glm/mat4x4.hpp"

#include "descriptor_writer.hpp"
#include "per_frame_links_on_data.hpp"

namespace ars_graphics
{
class VertexShaderData final
{
public:
    VertexShaderData(const ars_graphics::LogicalDevice& logical_device,
                     const ars_graphics::PhysicalDevice& physical_device,
                     uint32_t count_frames,
                     const DescriptorAllocator& descriptor_allocator);

    void updatePerFrameData(const ars_graphics::LogicalDevice& logical_device,
                            const ars_graphics::PhysicalDevice& physical_device,
                            uint32_t current_frame_index);

    void bind(const vk::CommandBuffer& buffer,
              vk::PipelineLayout pipeline_layout,
              uint32_t frame_index);

    void init(const ars_graphics::LogicalDevice& logical_device,
              const ars_graphics::PhysicalDevice& physical_device,
              const PerFrameLinksOnData& links);

    void updateUbo(const UboData& ubo);

private:
    void updateDescriptorSets(const ars_graphics::LogicalDevice& logical_device,
                              uint32_t current_frame_index);

    PerFrameLinksOnData m_links;
    UboData m_ubo;

    vk::DescriptorSetLayout m_layout;

    std::vector<UniformBuffer> m_uniform_buffers;
    std::vector<StorageBuffer> m_model_inst_storage_buffers;
    std::vector<StorageBuffer> m_mesh_inst_storage_buffers;
    std::vector<StorageBuffer> m_mapping_inst_storage_buffers;

    std::vector<vk::UniqueDescriptorSet> m_descriptors;

    DescriptorWriter m_descriptor_writer;
};
} // namespace ars_graphics
