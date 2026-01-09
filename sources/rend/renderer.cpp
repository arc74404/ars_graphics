#include "renderer.hpp"

namespace ars_graphics
{
RendererImpl::RendererImpl(const RendererConfigInfo& config_info)
    : m_instance(config_info.render_name),
      m_surface(config_info.interface_window->createSurface(m_instance.get())),
      m_physical_device(m_instance, m_surface),
      m_logical_device(m_physical_device.get(),
                       m_physical_device.getQueueFamilyIndices()),
      m_descriptor_manager(m_logical_device),
      m_pipeline_layout_storage(m_logical_device, m_descriptor_manager),
      m_model_manager(
          m_logical_device,
          m_physical_device,
          m_descriptor_manager,
          m_pipeline_layout_storage.getLayout(PipelineLayoutType::STANDART),
          config_info.models_paths),
      m_swapchain(m_logical_device,
                  m_physical_device,
                  m_surface,
                  config_info.interface_window->getWidth(),
                  config_info.interface_window->getHeight())
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
