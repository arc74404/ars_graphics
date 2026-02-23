#include "pipeline_layout_storage.hpp"

#include <optional>

namespace
{
std::optional<vk::UniquePipelineLayout>
createPipelineLayout(
    const ars_graphics::LogicalDevice& device,
    const std::vector<vk::DescriptorSetLayout>& descriptor_set_layouts,
    const std::vector<vk::PushConstantRange>& push_constant_ranges = {})
{
    vk::PipelineLayoutCreateInfo layout_info{};

    if (false == descriptor_set_layouts.empty())
    {
        layout_info.setSetLayouts(descriptor_set_layouts);
    }

    if (false == push_constant_ranges.empty())
    {
        layout_info.setPushConstantRanges(push_constant_ranges);
    }

    vk::UniquePipelineLayout pipeline_layout;

    auto&& res = device.get().createPipelineLayoutUnique(layout_info);

    if (res.result != vk::Result::eSuccess)
    {
        return std::nullopt;
    }
    return std::move(res.value);
}
} // namespace

namespace ars_graphics
{

vk::UniquePipelineLayout
createStandartPipelineLayout(const LogicalDevice& device,
                             const DescriptorManager& descriptor_manager)
{
    std::vector<vk::DescriptorSetLayout> layouts = {
        descriptor_manager.getAllocator(DescriptorSetLayoutType::MODEL_VERTEX)
            .layout(),
        descriptor_manager.getAllocator(DescriptorSetLayoutType::MODEL_MATERIAL)
            .layout()};

    auto&& res = createPipelineLayout(device, layouts);

    if (false == res.has_value())
    {
        throw std::runtime_error("Failed create standart pipeline layout");
    }
    return std::move(res.value());
}

PipelineLayoutStorage::PipelineLayoutStorage(
    const LogicalDevice& device,
    const DescriptorManager& descriptor_manager)
{
    m_storage[PipelineLayoutType::STANDART] =
        createStandartPipelineLayout(device, descriptor_manager);
}

const vk::PipelineLayout&
PipelineLayoutStorage::getLayout(PipelineLayoutType type) const
{
    return m_storage.at(type).get();
}

} // namespace ars_graphics
