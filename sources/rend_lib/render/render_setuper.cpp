#include "render_setuper.hpp"

#include "../descriptors/descriptor_pool.hpp"
#include "../descriptors/descriptor_set_layout.hpp"

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

std::vector<std::vector<SwapChainFrame>>
RenderSetuper::createAllFrames() const
{
    std::vector<std::vector<SwapChainFrame>> res;

    res.reserve(m_render_passes.size());

    for (auto&& rp : m_render_passes)
    {
        res.emplace_back(createFrames(
            m_logical_device, m_physical_device, m_swapchain,
            m_swapchain.getExtent(), m_command_pool, rp.get(),
            m_formats.m_depth_format, m_formats.m_surface_format.format));
    }
    return res;
}

RenderSetuper::RenderSetuper(const RenderSetuperConfigInfo& render_info)
    : m_instance(render_info.inst_name),
      m_surface(render_info.surface_creater.createSurface(m_instance)),
      m_physical_device(m_instance, m_surface),
      m_logical_device(m_physical_device,
                       m_physical_device.getQueueFamilyIndices()),
      m_command_pool(m_logical_device, m_physical_device),
      m_formats(m_physical_device.calculateSwapchainDepthFormat(),
                chooseSurfaceFormat(
                    m_physical_device.calculateSurfaceFormats(m_surface))),
      m_render_passes(createRenderPasses(m_logical_device,
                                         render_info.render_pass_configs,
                                         m_formats.m_surface_format.format,
                                         m_formats.m_depth_format)),
      m_swapchain(m_logical_device,
                  m_physical_device,
                  m_surface,
                  m_formats.m_depth_format,
                  m_formats.m_surface_format,
                  render_info.width,
                  render_info.height),
      m_frames(createAllFrames()),
      m_sync(m_logical_device, m_frames.size())
{
}

uint32_t
RenderSetuper::getFrameNumber() const
{
    return m_frame_number;
}

vk::RenderPass
RenderSetuper::getRenderPass(uint32_t render_pass_index) const
{
    return m_render_passes[render_pass_index].get();
}

const SwapChainFrame&
RenderSetuper::getFrame(uint32_t frame_index, uint32_t render_pass_index) const
{
    return m_frames[render_pass_index][frame_index];
}

const vk::Extent2D&
RenderSetuper::getExtent() const
{
    return m_swapchain.getExtent();
}

vk::Extent2D
RenderSetuper::getWindowSize() const
{
    vk::SurfaceCapabilitiesKHR capabilities =
        vk::PhysicalDevice(m_physical_device)
            .getSurfaceCapabilitiesKHR(m_surface)
            .value;
    return capabilities.currentExtent;
}

vk::Result
RenderSetuper::submit(uint32_t image_index, vk::CommandBuffer cmd)
{
    vk::SubmitInfo submit_info = {};

    vk::PipelineStageFlags wait_stages[] = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput};

    vk::Semaphore signal_semaphores[] = {
        m_sync.m_data[image_index].m_render_finished.get()};

    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores =
        &m_sync.m_data[m_frame_number].m_image_available.get();
    submit_info.pWaitDstStageMask    = wait_stages;
    submit_info.commandBufferCount   = 1;
    submit_info.pCommandBuffers      = &cmd;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores    = signal_semaphores;

    return m_logical_device.getQueue("graphics")
        .submit(submit_info,
                m_sync.m_data[m_frame_number].m_in_flight_fence.get());
}

vk::Result
RenderSetuper::present(uint32_t image_index)
{
    vk::PresentInfoKHR presentInfo{};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores =
        &(m_sync.m_data[image_index].m_render_finished.get());
    presentInfo.swapchainCount    = 1;
    vk::SwapchainKHR vk_swapchain = vk::SwapchainKHR(m_swapchain);
    presentInfo.pSwapchains       = &vk_swapchain;
    presentInfo.pImageIndices     = &image_index;

    vk::ResultValue<uint64_t> present_result = {vk::Result::eSuccess, 0};

    VkPresentInfoKHR vkPresentInfo = presentInfo;
    VkResult rawResult             = vkQueuePresentKHR(
        static_cast<VkQueue>(m_logical_device.getQueue("present")),
        &vkPresentInfo);

    return static_cast<vk::Result>(rawResult);
}
vk::Result
RenderSetuper::endRender(uint32_t image_index, vk::CommandBuffer cmd)
{
    auto&& submit_res = submit(image_index, cmd);

    if (submit_res != vk::Result::eSuccess)
    {
        return submit_res;
    }

    auto&& present_res = present(image_index);
    if (present_res != vk::Result::eSuccess)
    {
        return present_res;
    }

    if (present_res == vk::Result::eErrorOutOfDateKHR ||
        present_res == vk::Result::eSuboptimalKHR)
    {
        recreate(getWindowSize());
        return vk::Result::eSuccess;
    }

    m_frame_number = (m_frame_number + 1) % m_frames[0].size();
    return vk::Result::eSuccess;
}

uint32_t
RenderSetuper::getFramesCount() const
{
    return m_frames.size();
}

} // namespace ars_graphics
