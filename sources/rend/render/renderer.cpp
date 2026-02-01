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
      m_model_manager(m_logical_device,
                      m_physical_device,
                      m_descriptor_manager,
                      config_info.models_paths),
      m_sync(m_logical_device, m_swapchain.countFrames())
{
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
RendererImpl::startRenderPass(RenderPassType renderpass_type,
                              uint32_t image_index)
{
    vk::RenderPassBeginInfo renderPassInfo{};
    renderPassInfo.renderPass =
        m_renderpass_manager.getRenderPass(renderpass_type);
    renderPassInfo.framebuffer =
        m_swapchain[image_index].getFramebuffer(renderpass_type);
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

vk::Result
RendererImpl::submit(uint32_t image_index)
{
    vk::SubmitInfo submit_info = {};

    vk::PipelineStageFlags wait_stages[] = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput};

    vk::Semaphore signal_semaphores[] = {
        m_sync.m_data[image_index].m_render_finished.get()};

    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores =
        &m_sync.m_data[frame_number].m_image_available.get();
    submit_info.pWaitDstStageMask    = wait_stages;
    submit_info.commandBufferCount   = 1;
    submit_info.pCommandBuffers      = m_render_ctx.cmd;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores    = signal_semaphores;

    return m_logical_device.getQueue("graphics")
        .submit(submit_info,
                m_sync.m_data[frame_number].m_in_flight_fence.get());
}

vk::Result
RendererImpl::present(uint32_t image_index)
{
    vk::PresentInfoKHR presentInfo{};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores =
        &(m_sync.m_data[image_index].m_render_finished.get());
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains    = &m_swapchain.get();
    presentInfo.pImageIndices  = &image_index;

    vk::ResultValue<uint64_t> present_result = {vk::Result::eSuccess, 0};

    VkPresentInfoKHR vkPresentInfo = presentInfo;
    VkResult rawResult             = vkQueuePresentKHR(
        static_cast<VkQueue>(m_logical_device.getQueue("present")),
        &vkPresentInfo);

    return static_cast<vk::Result>(rawResult);
}

void
RendererImpl::recreate(const vk::Extent2D& window_size)
{
    std::cout << "RECREATE\n";
    m_logical_device.get().waitIdle();

    m_swapchain.destroy();
    m_swapchain = SwapChain(m_logical_device, m_physical_device, m_surface,
                            m_renderpass_manager, m_formats.m_depth_format,
                            m_formats.m_surface_format, window_size.width,
                            window_size.height);
}

void
RendererImpl::waitFence(uint8_t index)
{
    m_logical_device.get().waitForFences(
        1, &(m_sync.m_data[frame_number].m_in_flight_fence.get()), VK_TRUE,
        UINT64_MAX);
    m_logical_device.get().resetFences(
        1, &(m_sync.m_data[frame_number].m_in_flight_fence.get()));
}

vk::ResultValue<uint32_t>
RendererImpl::acquireNextImage(uint8_t index)
{
    return m_logical_device.get().acquireNextImageKHR(
        m_swapchain.get(), UINT64_MAX,
        m_sync.m_data[frame_number].m_image_available.get(), nullptr);
}

void
RendererImpl::draw(uint8_t index)
{
    m_render_info_data.vertex_buffer.bind(*m_render_ctx.cmd);
    m_render_info_data.index_buffer.bind(*(m_render_ctx.cmd));
    m_vertex_shader_ubo.bind(
        *(m_render_ctx.cmd),
        m_pipeline_manager.getLayout(PipelineLayoutType ::STANDART), index);

    for (auto&& per_primitive : m_render_info_data.per_primitive_data)
    {
        per_primitive.pipeline->bind(*m_render_ctx.cmd);
        per_primitive.need_init_in_calc.material->bind(
            *m_render_ctx.cmd,
            m_pipeline_manager.getLayout(PipelineLayoutType::STANDART));

        if (per_primitive.need_init_in_calc.has_indices)
        {
            m_render_info_data.index_buffer.draw(
                *(m_render_ctx.cmd),
                per_primitive.need_init_in_calc.index_buffer_data_info
                    .index_count,
                per_primitive.need_init_in_calc.index_buffer_data_info
                    .instance_count,
                per_primitive.need_init_in_calc.index_buffer_data_info
                    .first_index,
                per_primitive.need_init_in_calc.index_buffer_data_info
                    .vertex_offset,
                per_primitive.need_init_in_calc.index_buffer_data_info
                    .first_instance);
        }
        else
        {
            m_render_info_data.vertex_buffer.draw(
                *(m_render_ctx.cmd),
                per_primitive.need_init_in_calc.vertex_buffer_data_info
                    .vertex_count,
                per_primitive.need_init_in_calc.vertex_buffer_data_info
                    .instance_count,
                per_primitive.need_init_in_calc.vertex_buffer_data_info
                    .first_vertex,
                per_primitive.need_init_in_calc.vertex_buffer_data_info
                    .first_instance);
        }
    }
}

void
RendererImpl::render(const RenderingInfo& rendering_info)
{
    RenderPassType renderpass_type = RenderPassType::STANDART;

    waitFence(frame_number);

    vk::ResultValue<uint32_t> acquire = acquireNextImage(frame_number);

    // update buffers //

    if (rendering_info.camera.needRecalculation())
    {
        m_vertex_shader_ubo.updateCamera(rendering_info.camera.recalculate());
    }

    m_vertex_shader_ubo.updatePerFrameUboBuffer(
        m_logical_device, m_physical_device, frame_number);

    // -------------- //

    m_swapchain[frame_number].shareContext(m_render_ctx, renderpass_type);

    m_render_ctx.cmd->reset();

    vk::CommandBufferBeginInfo begin_info{};
    begin_info.pInheritanceInfo = nullptr;

    m_render_ctx.cmd->begin(&begin_info);

    startRenderPass(renderpass_type, acquire.value);

    setupScope();

    if (m_render_info_data.m_is_valid)
    {
        draw(acquire.value);
    }

    m_render_ctx.cmd->endRenderPass();

    m_render_ctx.cmd->end();

    auto&& submit_res = submit(acquire.value);

    auto&& present_res = present(acquire.value);

    if (present_res == vk::Result::eErrorOutOfDateKHR ||
        present_res == vk::Result::eSuboptimalKHR)
    {
        recreate(rendering_info.window_size);
        return;
    }

    frame_number = (frame_number + 1) % m_swapchain.countFrames();
}

} // namespace ars_graphics
