#include "renderer.hpp"

namespace ars_graphics
{
Renderer::Renderer(const RendererConfigInfo& config_info)
    : m_instance(config_info.render_name),
      m_surface(config_info.interface_window->createSurface(m_instance.get())),
      m_physical_device(m_instance, m_surface),
      m_logical_device(m_physical_device.get(),
                       m_physical_device.getQueueFamilyIndices()),
      m_model_manager(m_logical_device,
                      m_physical_device,
                      config_info.models_paths)
{
}
} // namespace ars_graphics
