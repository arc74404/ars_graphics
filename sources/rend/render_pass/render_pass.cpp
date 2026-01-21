#include "render_pass.hpp"

#include "render_pass_creater.hpp"

namespace ars_graphics
{

RenderPass::RenderPass(RenderPassCreater& creater,
                       const RenderPassConfigInfo& config_info)
{
    m_render_pass = creater.createRenderPass(config_info);
}

const vk::RenderPass&
RenderPass::get() const
{
    return m_render_pass.get();
}

} // namespace ars_graphics
