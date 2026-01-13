#include "material.hpp"

#include <iostream>

namespace ars_graphics
{

Material::Material(const LogicalDevice& device,
                   const DescriptorManager& desc_manager,
                   const vk::PipelineLayout& pipelayout,
                   const MaterialData& material_data)
    : m_pipeline_layout(pipelayout), m_data(material_data)
{
    desc_manager.getAllocator(DescriptorSetLayoutType::MATERIAL)
        .allocate(device, m_descriptor_set);
    setupDescriptorSets();
    updateDescriptorSets(device);
}

void
Material::addMap(const Texture* texture, uint32_t shift)
{
    if (texture)
    {
        vk::DescriptorImageInfo text_info{};
        text_info.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        text_info.imageView   = texture->getImageView();
        text_info.sampler     = texture->getSampler();

        m_image_infos.push_back(text_info);

        m_descriptor_writes.push_back(vk::WriteDescriptorSet(
            m_descriptor_set.get(),
            settings::bindings::material_shader_binding + shift, 0, 1,
            vk::DescriptorType::eCombinedImageSampler, &m_image_infos.back(),
            nullptr, nullptr));
    }
}

void
Material::setupDescriptorSets()
{
    m_image_infos.clear();
    m_descriptor_writes.clear();

    addMap(m_data.m_pbrparams.m_albedo_map, 0);
    addMap(m_data.m_pbrparams.m_normal_map, 1);
    addMap(m_data.m_pbrparams.m_metallic_roughness_map, 2);
    addMap(m_data.m_pbrparams.m_ao_map, 3);
    addMap(m_data.m_pbrparams.m_emissive_map, 4);
    addMap(m_data.m_pbrparams.m_height_map, 5);
}

void
Material::updateDescriptorSets(const LogicalDevice& device)
{
    device.get().updateDescriptorSets(
        static_cast<uint32_t>(m_descriptor_writes.size()),
        m_descriptor_writes.data(), 0, nullptr);
}

void
Material::bind(const vk::CommandBuffer& cmd) const
{
    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipeline_layout,
                           1u, 1, &(m_descriptor_set.get()), 0, nullptr);
}
} // namespace ars_graphics
