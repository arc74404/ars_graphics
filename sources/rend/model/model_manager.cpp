#include "model_manager.hpp"

#include "../material/material_creater.hpp"

namespace ars_graphics
{
ModelManager::ModelManager(PipelineManager& pipeline_manager,
                           const vk::RenderPass& render_pass,
                           const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device,
                           const DescriptorManager& descriptor_manager,
                           const vk::PipelineLayout& pipelayout,
                           const std::vector<std::string>& paths)
    : m_texture_storage(logical_device, physical_device)
{
    TextureCreater texture_creater{logical_device, physical_device};

    MaterialCreater material_creater(logical_device, descriptor_manager,
                                     pipelayout);

    m_loader.load(pipeline_manager, render_pass, m_texture_storage,
                  texture_creater, m_material_storage, material_creater, paths,
                  m_models);
}

const Model*
ModelManager::operator[](const std::string& key)
{
    auto&& it = m_models.find(key);
    if (it == m_models.end())
    {
        throw std::runtime_error(std::format("{}{}", "No such model: ", key));
    }
    return &(it->second);
}

} // namespace ars_graphics
