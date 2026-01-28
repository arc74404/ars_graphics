#pragma once

#include "pipeline_creater.hpp"

namespace ars_graphics
{
class DynamicViewportPipelineCreater : public PipelineCreater
{
public:
    virtual ~DynamicViewportPipelineCreater() = default;

    DynamicViewportPipelineCreater(const LogicalDevice& device,
                                   const ShaderManager& shader_manager);

    vk::PipelineViewportStateCreateInfo viewportState(
        const PipelineConfigInfo& config_info,
        vk::Viewport& viewport,
        vk::Rect2D& scissors) override;

private:
    vk::PipelineDynamicStateCreateInfo dynamicStates(
        const PipelineConfigInfo& config_info) override;


};
} // namespace ars_graphics
