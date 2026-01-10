#include "renderer.hpp"

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

void
RendererImpl::clear()
{
    m_swapchain.destroy();
    m_instance.get().destroySurfaceKHR(m_surface);
}

} // namespace ars_graphics
