#include "pipeline.hpp"

#include "pipeline_creater.hpp"

namespace ars_graphics
{
void
Pipeline::bind(const vk::CommandBuffer& cmd) const
{
    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline.get());
}

Pipeline::Pipeline(PipelineCreater& creater,
                   const vk::RenderPass& render_pass,
                   const PipelineConfigInfo& pipeline_config_info)
{
    m_pipeline =
        std::move(creater.createPipeline(render_pass, pipeline_config_info));
}

} // namespace ars_graphics
