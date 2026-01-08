#include "renderer.hpp"

namespace ars_graphics
{
Renderer::Renderer(const RendererConfigInfo& config_info)
    : m_instance(config_info.render_name),
      m_surface(std::move(
          config_info.interface_window->createSurface(m_instance.get()))),
      m_physical_device(m_instance, m_surface.get()),
      m_logical_device(m_physical_device.get(),
                       m_physical_device.getQueueFamilyIndices()),
      m_descriptor_manager(m_logical_device),
      m_pipeline_layout_storage(m_logical_device, m_descriptor_manager),
      m_model_manager(
          m_logical_device,
          m_physical_device,
          m_descriptor_manager,
          m_pipeline_layout_storage.getLayout(PipelineLayoutType::STANDART),
          config_info.models_paths)
{
    std::cout << "All RIght!\n";
}
} // namespace ars_graphics
