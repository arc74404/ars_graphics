#include "pipeline_manager.hpp"

namespace ars_graphics
{
PipelineManager::PipelineManager(const LogicalDevice& device,
                                 const ShaderManager& shader_manager,
                                 const DescriptorManager& descriptor_manager,
                                 const vk::Extent2D& extent)
    : m_creater(device, shader_manager),
      m_extent(extent),
      m_layout_storage(device, descriptor_manager)
{
    m_storages[PipelineStorageType::STANDART_MODEL];
}

const vk::PipelineLayout&
PipelineManager::getLayout(PipelineLayoutType type) const
{
    return m_layout_storage.getLayout(type);
}

} // namespace ars_graphics
