#include "material.hpp"

namespace ars_graphics
{

Material::Material(const LogicalDevice& device,
                   const DescriptorManager& desc_manager,
                   const vk::PipelineLayout& pipelayout,
                   const PBRParameters& pbr)
    : m_pipeline_layout(pipelayout),
      m_pbr(pbr),
      m_count_textures(pbr.countTextures())
{
    desc_manager.getAllocator(DescriptorSetLayoutType::MATERIAL)
        .allocate(device, m_descriptor_set);
}
void
Material::bind(const vk::CommandBuffer& cmd) const
{
    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipeline_layout,
                           1u, 1, &(m_descriptor_set.get()), 0, nullptr);
}

} // namespace ars_graphics
