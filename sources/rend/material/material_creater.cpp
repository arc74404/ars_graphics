#include "material_creater.hpp"

namespace ars_graphics
{

MaterialCreater::MaterialCreater(const LogicalDevice& logical_device,
                                 const PhysicalDevice& physical_device)
    : m_logical_device{logical_device}, m_physical_device{physical_device}
{
}

std::vector<Material>
MaterialCreater::convertToMaterials(
    const std::vector<MaterialData>& mdata) const
{
    std::vector<Material> materials;

    for (auto&& data : mdata)
    {
        materials.emplace_back(m_logical_device, m_physical_device,
                               m_descriptor_manager, data);
    }

    return materials;
}
} // namespace ars_graphics
