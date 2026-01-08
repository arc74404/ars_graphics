#include "model_manager.hpp"

#include "../material/material_creater.hpp"

namespace ars_graphics
{
ModelManager::ModelManager(const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device,
                           const DescriptorManager& descriptor_manager,
                           vk::PipelineLayout pipelayout,
                           const std::vector<std::string>& paths)
{
    TextureCreater texture_creater{logical_device, physical_device};

    MaterialCreater material_creater(logical_device, descriptor_manager,
                                     pipelayout);

    m_loader.load(m_texture_data_storage, m_texture_storage, texture_creater,
                  m_material_storage, material_creater, paths, m_models);
}
} // namespace ars_graphics
