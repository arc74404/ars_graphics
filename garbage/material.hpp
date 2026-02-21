#pragma once

#include <iostream>
#include <string>

#include "../buffers/uniform_buffer.hpp"
#include "../descriptors/descriptor_manager.hpp"
#include "../textures/texture.hpp"
#include "../vertex/vertex.hpp"

#include "material_data.hpp"

namespace ars_graphics
{

template <typename MaterialDataType>
class Material final
{
public:
    Material(const LogicalDevice& logical_device,
             const PhysicalDevice& physical_device,
             const MaterialDataType& material_data,
             vk::UniqueDescriptorSet&& descriptor_set);

    static Material createDefaultMaterial();

    void updateDescriptorSets(const LogicalDevice& device);

    void bind(const vk::CommandBuffer& cmd,
              const vk::PipelineLayout& layout) const;

private:
    // void setupParamsBuffer(const LogicalDevice& logical_device,
    //                        const PhysicalDevice& physical_device);
    // void setupDescriptorSets();
    // void addMap(const Texture* texture, uint32_t shift);
    // void addParams(uint32_t shift);

    MaterialDataType m_material_data;

    std::shared_ptr<UniformBuffer> m_shader_params_buffer;
    std::shared_ptr<vk::DescriptorBufferInfo> m_shader_params_buffer_info;

    vk::UniqueDescriptorSet m_descriptor_set;

    std::vector<vk::DescriptorImageInfo> m_image_infos;

    std::vector<vk::WriteDescriptorSet> m_descriptor_writes;
};
} // namespace ars_graphics
