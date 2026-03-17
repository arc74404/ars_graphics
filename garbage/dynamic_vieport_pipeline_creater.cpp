#include "dynamic_vieport_pipeline_creater.hpp"

#include "../device/logical_device.hpp"

namespace ars_graphics
{
DynamicViewportPipelineCreater::DynamicViewportPipelineCreater(
    const LogicalDevice& device,
    const ShaderManager& shader_manager)
    : PipelineCreater(device, shader_manager)
{
    m_dynamic_states = {vk::DynamicState::eScissor,
                        vk::DynamicState::eViewport};
}

vk::PipelineViewportStateCreateInfo
DynamicViewportPipelineCreater::viewportState(
    const PipelineConfigInfo& config_info,
    vk::Viewport& viewport,
    vk::Rect2D& scissors)
{
    vk::PipelineViewportStateCreateInfo viewportState{};
    viewportState.viewportCount = 1;
    viewportState.scissorCount  = 1;
    viewportState.pViewports    = nullptr;
    viewportState.pScissors     = nullptr;
    return viewportState;
}

vk::PipelineDynamicStateCreateInfo
DynamicViewportPipelineCreater::dynamicStates(
    const PipelineConfigInfo& config_info)
{
    vk::PipelineDynamicStateCreateInfo res;

    res.dynamicStateCount = m_dynamic_states.size();
    res.pDynamicStates    = m_dynamic_states.data();

    return res;
}

} // namespace ars_graphics
