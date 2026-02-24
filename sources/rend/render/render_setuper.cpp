#include "render_setuper.hpp"

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

} // namespace ars_graphics
