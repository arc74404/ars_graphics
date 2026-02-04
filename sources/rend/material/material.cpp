#include "material.hpp"

#include "../shaders/shaders_data_structs/material_params.hpp"
#include "../textures/texture_storage.hpp"

namespace ars_graphics
{

Material::Material(const LogicalDevice& logical_device,
                   const PhysicalDevice& physical_device,
                   const DescriptorManager& desc_manager,
                   const MaterialData& material_data)
    : m_material_data(material_data),
      m_shader_params_buffer_info(std::make_shared<vk::DescriptorBufferInfo>()),
      m_shader_params_buffer(std::make_shared<UniformBuffer>())
{
    setupParamsBuffer(logical_device, physical_device);
    desc_manager.getAllocator(DescriptorSetLayoutType::MODEL_MATERIAL)
        .allocate(logical_device, m_descriptor_set);
    setupDescriptorSets();
    updateDescriptorSets(logical_device);
}

void
Material::setupParamsBuffer(const LogicalDevice& logical_device,
                            const PhysicalDevice& physical_device)
{
    shaders_params::MaterialParams params = {
        .albedo_factor = m_material_data.m_pbrparams.m_albedo_color,
        .emissive_factor =
            glm::vec4(m_material_data.m_pbrparams.m_emissive_factor, 1),
        .metallic    = m_material_data.m_pbrparams.m_metallic,
        .roughness   = m_material_data.m_pbrparams.m_roughness,
        .ao_strength = m_material_data.m_pbrparams.m_ao_strength};

    m_shader_params_buffer->setData(logical_device, physical_device, &params,
                                    sizeof(shaders_params::MaterialParams));
}

void
Material::addMap(const Texture* texture, uint32_t shift)
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

void
Material::addParams(uint32_t shift)
{
    m_shader_params_buffer_info->buffer = m_shader_params_buffer->buffer();
    m_shader_params_buffer_info->offset = 0;
    m_shader_params_buffer_info->range  = m_shader_params_buffer->byteSize();

    vk::WriteDescriptorSet buffer_write{};

    buffer_write.dstSet = m_descriptor_set.get();
    buffer_write.dstBinding =
        settings::bindings::material_shader_binding + shift;
    buffer_write.dstArrayElement  = 0;
    buffer_write.descriptorCount  = 1;
    buffer_write.descriptorType   = vk::DescriptorType::eUniformBuffer;
    buffer_write.pImageInfo       = nullptr;
    buffer_write.pBufferInfo      = m_shader_params_buffer_info.get();
    buffer_write.pTexelBufferView = nullptr;

    m_descriptor_writes.push_back(buffer_write);
}

void
Material::setupDescriptorSets()
{
    m_image_infos.clear();
    m_descriptor_writes.clear();

    m_image_infos.reserve(6);
    m_descriptor_writes.reserve(6);

    addMap(m_material_data.m_pbrparams.m_albedo_map, 0);
    addMap(m_material_data.m_pbrparams.m_normal_map, 1);
    addMap(m_material_data.m_pbrparams.m_metallic_roughness_map, 2);
    addMap(m_material_data.m_pbrparams.m_ao_map, 3);
    addMap(m_material_data.m_pbrparams.m_emissive_map, 4);
    addParams(5);
}

void
Material::updateDescriptorSets(const LogicalDevice& device)
{
    device.get().updateDescriptorSets(
        static_cast<uint32_t>(m_descriptor_writes.size()),
        m_descriptor_writes.data(), 0, nullptr);
}

void
Material::bind(const vk::CommandBuffer& cmd,
               const vk::PipelineLayout& layout) const
{
    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 1u, 1,
                           &(m_descriptor_set.get()), 0, nullptr);
}
} // namespace ars_graphics
