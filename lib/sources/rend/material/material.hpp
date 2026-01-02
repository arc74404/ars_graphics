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
class Material
{
public:
    Material(const LogicalDevice& device,
             const DescriptorManager& desc_manager,
             const vk::PipelineLayout& pipelayout,
             const PBRParameters& pbr);

    void bind(const vk::CommandBuffer& cmd) const;

private:
    uint8_t m_count_textures;

    PBRParameters m_pbr;

    vk::UniqueDescriptorSet m_descriptor_set;

    const vk::PipelineLayout& m_pipeline_layout;
};
} // namespace ars_graphics
