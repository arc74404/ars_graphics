#include "renderer.hpp"

#include "../../gui/window/interface_window.hpp"

namespace ars_graphics
{

namespace
{
vk::SurfaceFormatKHR
chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats)
{
    for (vk::SurfaceFormatKHR format : available_formats)
    {
        if (format.format == vk::Format::eB8G8R8A8Unorm &&
            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return format;
        }
    }

    return available_formats[0];
}
} // namespace

RendererImpl::RendererImpl(const RendererConfigInfo& config_info)
    : m_instance(config_info.render_name),
      m_surface(config_info.interface_window->createSurface(m_instance.get())),
      m_physical_device(m_instance, m_surface),
      m_logical_device(m_physical_device.get(),
                       m_physical_device.getQueueFamilyIndices()),
      m_command_pool_controler(m_logical_device, m_physical_device),
      m_descriptor_manager(m_logical_device),
      m_formats(m_physical_device.calculateSwapchainDepthFormat(),
                chooseSurfaceFormat(
                    m_physical_device.calculateSurfaceFormats(m_surface))),
      m_renderpass_manager(m_logical_device,
                           m_formats.m_surface_format.format,
                           m_formats.m_depth_format),
      m_swapchain(m_logical_device,
                  m_physical_device,
                  m_surface,
                  m_renderpass_manager,
                  m_formats.m_depth_format,
                  m_formats.m_surface_format,
                  config_info.interface_window->getWidth(),
                  config_info.interface_window->getHeight()),
      m_shader_manager(m_logical_device, config_info.shader_paths),
      m_pipeline_manager(m_logical_device,
                         m_shader_manager,
                         m_descriptor_manager,
                         m_swapchain.getExtent()),
      m_vertex_shader_ubo(m_logical_device,
                          m_physical_device,
                          m_swapchain.countFrames(),
                          m_descriptor_manager.getAllocator(
                              DescriptorSetLayoutType::UBO_AND_STORAGE)),
      m_model_manager(
          m_pipeline_manager,
          m_renderpass_manager.getRenderPass(RenderPassType::STANDART),
          m_logical_device,
          m_physical_device,
          m_descriptor_manager,
          m_pipeline_manager.getLayout(PipelineLayoutType::STANDART),
          config_info.models_paths)

{
    std::cout << "All RIght!\n";
}

const Model*
RendererImpl::getModel(const std::string& name)
{
    return m_model_manager[name];
}

void
RendererImpl::clear()
{
    if (m_logical_device.get().waitIdle() != vk::Result::eSuccess)
    {
        std::cout << "Failed wait idle\n";
    }

    m_swapchain.destroy();
    m_command_pool_controler.destroy();
    m_instance.get().destroySurfaceKHR(m_surface);
}

void
RendererImpl::startRenderPass(RenderPassType renderpass_type)
{
    vk::RenderPassBeginInfo renderPassInfo{};
    renderPassInfo.renderPass =
        m_renderpass_manager.getRenderPass(renderpass_type);
    renderPassInfo.framebuffer       = *(m_render_ctx.framebuffer);
    renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
    renderPassInfo.renderArea.extent = m_swapchain.getExtent();

    std::array<vk::ClearValue, 2> clearValues{};
    clearValues[0].color =
        vk::ClearColorValue(std::array<float, 4>{0.1f, 0.1f, 0.1f, 1.0f});
    clearValues[1].depthStencil    = vk::ClearDepthStencilValue{1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues    = clearValues.data();

    m_render_ctx.cmd->beginRenderPass(&renderPassInfo,
                                      vk::SubpassContents::eInline);
}

void
RendererImpl::setupScope()
{
    vk::Viewport viewport{};
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = static_cast<float>(m_swapchain.getExtent().width);
    viewport.height   = static_cast<float>(m_swapchain.getExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    m_render_ctx.cmd->setViewport(0, 1, &viewport);

    vk::Rect2D scissor{};
    scissor.offset = vk::Offset2D{0, 0};
    scissor.extent = m_swapchain.getExtent();
    m_render_ctx.cmd->setScissor(0, 1, &scissor);
}

void
RendererImpl::present(const SynchronizationData& sync, uint32_t image_index)
{
    vk::PresentInfoKHR presentInfo{};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = &(sync.getRenderFinished());
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = &m_swapchain.get();
    presentInfo.pImageIndices      = &image_index;

    m_logical_device.getQueue("present").presentKHR(presentInfo);
}

void
RendererImpl::render(const RenderingInfo& rendering_info)
{
    RenderPassType renderpass_type = RenderPassType::STANDART;

    std::pair<const ars_graphics::SwapChainFrame&, uint32_t> frame_and_index =
        m_swapchain.currentFrame();

    auto&& synchronization = frame_and_index.first.getSynchronization();

    synchronization.waitForFence(m_logical_device);

    // update buffers //

    if (rendering_info.camera.needRecalculation())
    {
        m_vertex_shader_ubo.updateCamera(rendering_info.camera.recalculate());
    }

    m_vertex_shader_ubo.updatePerFrameUboBuffer(
        m_logical_device, m_physical_device, frame_and_index.second);

    // -------------- //

    uint32_t image_index =
        synchronization.acquireNextImage(m_logical_device, m_swapchain);

    frame_and_index.first.shareContext(m_render_ctx, renderpass_type);

    vk::CommandBufferBeginInfo begin_info{};
    begin_info.pInheritanceInfo = nullptr;

    m_render_ctx.cmd->begin(&begin_info);

    startRenderPass(renderpass_type);

    setupScope();

    if (m_render_info_data.m_is_valid)
    {
        m_render_info_data.vertex_buffer.bind(*m_render_ctx.cmd);
        m_render_info_data.index_buffer.bind(*(m_render_ctx.cmd));
        m_vertex_shader_ubo.bind(
            *(m_render_ctx.cmd),
            m_pipeline_manager.getLayout(PipelineLayoutType ::STANDART),
            image_index);

        for (auto&& per_primitive : m_render_info_data.per_primitive_data)
        {
            per_primitive.pipeline->bind(*m_render_ctx.cmd);
            per_primitive.material->bind(*m_render_ctx.cmd);

            if (per_primitive.has_indices)
            {
                m_render_info_data.index_buffer.draw(
                    *(m_render_ctx.cmd),
                    per_primitive.index_buffer_data_info.index_count,
                    per_primitive.index_buffer_data_info.instance_count,
                    per_primitive.index_buffer_data_info.first_index,
                    per_primitive.index_buffer_data_info.vertex_offset,
                    per_primitive.index_buffer_data_info.first_instance);
            }
            else
            {
                m_render_info_data.vertex_buffer.draw(
                    *(m_render_ctx.cmd),
                    per_primitive.vertex_buffer_data_info.vertex_count,
                    per_primitive.vertex_buffer_data_info.instance_count,
                    per_primitive.vertex_buffer_data_info.first_vertex,
                    per_primitive.vertex_buffer_data_info.first_instance);
            }
        }
    }

    m_render_ctx.cmd->endRenderPass();

    m_render_ctx.cmd->end();

    synchronization.submit(m_logical_device, *m_render_ctx.cmd);

    present(synchronization, image_index);
}

} // namespace ars_graphics
