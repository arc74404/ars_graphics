#pragma once

#include <map>

#include "../descriptors/descriptor_manager.hpp"
#include "../device/logical_device.hpp"

namespace ars_graphics
{
enum class PipelineLayoutType
{
    STANDART
};

class PipelineLayoutStorage final
{
public:
    PipelineLayoutStorage(const LogicalDevice& device,
                          const DescriptorManager& descriptor_manager);

    const vk::PipelineLayout& getLayout(PipelineLayoutType type) const;

private:
    std::map<PipelineLayoutType, vk::UniquePipelineLayout> m_storage;
};
} // namespace ars_graphics
