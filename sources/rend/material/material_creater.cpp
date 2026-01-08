#include "material_creater.hpp"

namespace ars_graphics
{

MaterialCreater::MaterialCreater(const LogicalDevice& logical_device,
                                 const DescriptorManager& descriptor_manager,
                                 const vk::PipelineLayout& pipeline_layout)
    : m_logical_device{logical_device},
      m_descriptor_manager{descriptor_manager},
      m_pipeline_layout{pipeline_layout}
{
}

std::vector<Material>
MaterialCreater::convertToMaterials(std::vector<MaterialData>&& mdata) const
{
    std::vector<Material> materials;

    for (auto&& data : mdata)
    {
        materials.emplace_back(m_logical_device, m_descriptor_manager,
                               m_pipeline_layout, data);
    }

    return materials;
}
} // namespace ars_graphics
