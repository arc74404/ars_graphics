#pragma once

#include "../descriptors/descriptor_manager.hpp"
#include "../device/logical_device.hpp"

#include "material.hpp"

namespace ars_graphics
{
class MaterialCreater final
{
public:
    MaterialCreater(const LogicalDevice& logical_device,
                    const PhysicalDevice& physical_device);

    std::vector<Material> convertToMaterials(
        const std::vector<MaterialData>& mdata) const;

private:
    const LogicalDevice& m_logical_device;
    const PhysicalDevice& m_physical_device;
};
} // namespace ars_graphics
