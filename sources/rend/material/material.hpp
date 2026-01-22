#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <iostream>
#include <string>

#include "../buffers/uniform_buffer.hpp"
#include "../descriptors/descriptor_manager.hpp"
#include "../textures/texture.hpp"
#include "../vertex/vertex.hpp"

#include "pbrparams.hpp"

namespace ars_graphics
{

enum class AlphaMode
{
    MASK,
    BLEND,
    TOTAL_OPAQUE
};

struct MaterialData
{
    PBRParameters m_pbrparams{};
    AlphaMode m_alpha_mode{};
    float m_alpha_cutoff{};
    bool m_double_sided{};
};

class Material final
{
public:
    Material(const LogicalDevice& logical_device,
             const PhysicalDevice& physical_device,
             const DescriptorManager& desc_manager,
             const vk::PipelineLayout& pipelayout,
             const MaterialData& material_data);

    static Material createDefaultMaterial();

    void updateDescriptorSets(const LogicalDevice& device);

    void bind(const vk::CommandBuffer& cmd) const;

private:
    void setupParamsBuffer(const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device);

    void setupDescriptorSets();

    void addMap(const Texture* texture, uint32_t shift);

    void addParams(uint32_t shift);

    MaterialData m_material_data;

    std::shared_ptr<UniformBuffer> m_shader_params_buffer;
    std::shared_ptr<vk::DescriptorBufferInfo> m_shader_params_buffer_info;

    vk::UniqueDescriptorSet m_descriptor_set;

    std::vector<vk::DescriptorImageInfo> m_image_infos;

    std::vector<vk::WriteDescriptorSet> m_descriptor_writes;

    const vk::PipelineLayout& m_pipeline_layout;
};
} // namespace ars_graphics
