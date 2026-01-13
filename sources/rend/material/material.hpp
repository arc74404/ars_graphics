#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <string>

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
    PBRParameters m_pbrparams;
    AlphaMode m_alpha_mode;
    float m_alpha_cutoff;
    bool m_double_sided;
};

class Material
{
public:
    Material(const LogicalDevice& device,
             const DescriptorManager& desc_manager,
             const vk::PipelineLayout& pipelayout,
             const MaterialData& material_data);

    static Material createDefaultMaterial();

    void updateDescriptorSets(const LogicalDevice& device);

    void bind(const vk::CommandBuffer& cmd) const;

private:
    void setupDescriptorSets();

    void addMap(const Texture* texture, uint32_t shift);

    MaterialData m_data;

    vk::UniqueDescriptorSet m_descriptor_set;

    std::vector<vk::DescriptorImageInfo> m_image_infos;
    std::vector<vk::WriteDescriptorSet> m_descriptor_writes;

    const vk::PipelineLayout& m_pipeline_layout;
};
} // namespace ars_graphics
