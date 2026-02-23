#include "render_impl.hpp"

namespace ars_graphics
{

static vk::SurfaceFormatKHR
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

std::vector<vk::UniqueRenderPass>
RendererImpl::createRenderPasses(
    const std::vector<RenderPassConfigInfo>& render_pass_configs) const
{
    std::vector<vk::UniqueRenderPass> render_passes;

    RenderPassBuilder render_pass_builder(m_logical_device);

    for (auto&& config : render_pass_configs)
    {
        render_passes.emplace_back(
            std::move(render_pass_builder.createRenderPass(config)));
    }

    return render_passes;
}

vk::UniqueFramebuffer
RendererImpl::createFramebuffer(vk::RenderPass render_pass,
                                vk::ImageView view,
                                vk::ImageView depth_view) const
{
    std::vector<vk::ImageView> attachments = {view, depth_view};

    vk::FramebufferCreateInfo config_info;

    config_info.flags           = vk::FramebufferCreateFlags();
    config_info.renderPass      = render_pass;
    config_info.attachmentCount = attachments.size();
    config_info.pAttachments    = attachments.data();
    config_info.width           = m_swapchain.getExtent().width;
    config_info.height          = m_swapchain.getExtent().height;
    config_info.layers          = 1;

    auto&& frmbuf =
        vk::Device(m_logical_device).createFramebufferUnique(config_info);

    if (frmbuf.result != vk::Result::eSuccess)
    {
        throw std::runtime_error("Failed create framebuffer");
    }

    return std::move(frmbuf.value);
}

std::vector<SwapChainFrame>
RendererImpl::createFrames(vk::RenderPass render_pass) const
{
    std::vector<SwapChainFrame> res;

    auto&& images =
        vk::Device(m_logical_device).getSwapchainImagesKHR(m_swapchain);

    if (false == images.has_value())
    {
        throw std::runtime_error("Failed getSwapchainImagesKHR in swapchain");
    }

    for (auto&& image : images.value)
    {
        FrameBuffer framebuffer;
        framebuffer.depth_image = Image(
            m_logical_device, m_physical_device,
            ImageConfigInfo{
                .format      = m_formats.m_depth_format,
                .width       = m_swapchain.getExtent().width,
                .height      = m_swapchain.getExtent().height,
                .usage_flags = vk::ImageUsageFlagBits::eDepthStencilAttachment |
                               vk::ImageUsageFlagBits::eTransferDst |
                               vk::ImageUsageFlagBits::eSampled,
                .aspect_mask = vk::ImageAspectFlagBits::eDepth});

        auto&& image_view_res = createImageView(
            m_logical_device, image, m_formats.m_surface_format.format,
            vk::ImageAspectFlagBits::eColor);

        framebuffer.view = std::move(image_view_res.value());
        framebuffer.m_framebuffer =
            createFramebuffer(render_pass, framebuffer.view.get(),
                              framebuffer.depth_image.view());

        res.emplace_back(m_command_pool.allocateCommandBuffer(m_logical_device),
                         std::move(framebuffer));
    }
    return res;
}

std::vector<std::vector<SwapChainFrame>>
RendererImpl::createAllFrames() const
{
    std::vector<std::vector<SwapChainFrame>> res;

    res.reserve(m_render_passes.size());

    for (auto&& rp : m_render_passes)
    {
        res.emplace_back(createFrames(rp.get()));
    }
    return res;
}

RendererImpl::RendererImpl(const RenderConfigInfo& render_info)
    : m_instance(render_info.inst_name),
      m_surface(render_info.window.createSurface(m_instance)),
      m_physical_device(m_instance, m_surface),
      m_logical_device(m_physical_device.get(),
                       m_physical_device.getQueueFamilyIndices()),
      m_command_pool(m_logical_device, m_physical_device),
      m_formats(m_physical_device.calculateSwapchainDepthFormat(),
                chooseSurfaceFormat(
                    m_physical_device.calculateSurfaceFormats(m_surface))),
      m_render_passes(createRenderPasses(render_info.render_pass_configs)),
      m_swapchain(m_logical_device,
                  m_physical_device,
                  m_surface,
                  m_formats.m_depth_format,
                  m_formats.m_surface_format,
                  render_info.window.getWidth(),
                  render_info.window.getHeight()),
      m_frames(createAllFrames()),
      m_sync(m_logical_device, m_frames.size())
{
}

void
RendererImpl::bindScene(IScene& scene)
{
}

void
RendererImpl::startRenderPass(uint32_t image_index)
{
    vk::RenderPassBeginInfo render_pass_info{};
    render_pass_info.renderPass  = m_render_pass.get();
    render_pass_info.framebuffer = m_swapchain[image_index].getFramebuffer();
    render_pass_info.renderArea.offset = vk::Offset2D{0, 0};
    render_pass_info.renderArea.extent = m_swapchain.getExtent();

    std::array<vk::ClearValue, 2> clear_values{};
    clear_values[0].color =
        vk::ClearColorValue(std::array<float, 4>{0.1f, 0.1f, 0.1f, 1.0f});
    clear_values[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};
    render_pass_info.clearValueCount =
        static_cast<uint32_t>(clear_values.size());
    render_pass_info.pClearValues = clear_values.data();

    m_render_ctx.cmd->beginRenderPass(&render_pass_info,
                                      vk::SubpassContents::eInline);
}

void
RendererImpl::render()
{
    waitFence(frame_number);

    vk::ResultValue<uint32_t> acquire = acquireNextImage(frame_number);

    // update buffers //
    // if (rendering_info.camera.needRecalculation())
    // {
    //     UboData ubo;
    //     ubo.camera_matrix = rendering_info.camera.recalculate();
    //     m_vertex_shader_data.updateUbo(ubo);
    // }
    // m_vertex_shader_data.updatePerFrameData(m_logical_device,
    // m_physical_device,
    //                                         frame_number);
    // -------------- //
    updatePerFrameBuffers();

    auto&& cmd = m_swapchain[frame_number].getCmd();

    cmd.reset();

    vk::CommandBufferBeginInfo begin_info{};
    begin_info.pInheritanceInfo = nullptr;

    cmd->begin(&begin_info);

    startRenderPass(acquire.value);

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
