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

template <typename... VertexAttributes>
class Material
{
public:
    using VertexType = Vertex<VertexAttributes...>;

    Material(const DescriptorManager& desc_manager,
             const vk::PipelineLayout& pipelayout,
             const PBRParameters& pbr)
        : m_pipeline_layout(pipelayout),
          m_pbr(pbr),
          m_count_textures(pbr.countTextures())
    {
        m_descriptor_set =
            desc_manager.getAllocator(DescriptorSetLayoutType::MATERIAL)
                .allocate(logical_device);
    }

    void bind(const vk::CommandBuffer& cmd) const
    {
        command_buffer.bindDescriptorSets(
            vk::PipelineBindPoint::eGraphics, m_pipeline_layout, 1u, 1,
            &(m_descriptor_set.get()), 0, nullptr);
    }

private:
    uint8_t m_count_textures;

    PBRParameters m_pbr;

    vk::UniqueDescriptorSet m_descriptor_set;

    const vk::PipelineLayout& m_pipeline_layout;
};
} // namespace ars_graphics
