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
                    const PhysicalDevice& physical_device,
                    const DescriptorManager& descriptor_manager);

    std::vector<Material> convertToMaterials(
        const std::vector<MaterialData>& mdata) const;

private:
    const LogicalDevice& m_logical_device;
    const PhysicalDevice& m_physical_device;
    const DescriptorManager& m_descriptor_manager;
};
} // namespace ars_graphics
