#include "pipeline.hpp"

#include "pipeline_creater.hpp"

namespace ars_graphics
{
void
Pipeline::bind(const vk::CommandBuffer& cmd) const
{
    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline.get());
}

Pipeline::Pipeline(std::unique_ptr<PipelineCreater>&& creater,
                   const PipelineConfigInfo& pipeline_config_info,
                   const vk::RenderPass& render_pass,
                   const ShaderManager& shader_manager,
                   const LogicalDevice& device)
{
    creater->init(pipeline_config_info);
    m_pipeline =
        std::move(creater->createPipeline(shader_manager, render_pass, device));
}

} // namespace ars_graphics
