#pragma once
#include <iostream>
#include <vector>

#include "../window/interface_window.hpp"
#include "device/logical_device.hpp"
#include "device/physical_device.hpp"
#include "instance/instance.hpp"
#include "model/model_manager.hpp"
#include "pipelines/pipeline_layout_storage.hpp"

namespace ars_graphics
{
struct RendererConfigInfo final
{
    IWindow* interface_window;
    std::string render_name;
    std::vector<std::string> models_paths;
};

class Renderer final
{
public:
    Renderer(const RendererConfigInfo& config_info);

private:
    Instance m_instance;

    vk::UniqueSurfaceKHR m_surface;

    PhysicalDevice m_physical_device;

    LogicalDevice m_logical_device;

    DescriptorManager m_descriptor_manager;

    PipelineLayoutStorage m_pipeline_layout_storage;

    ModelManager m_model_manager;
};
}; // namespace ars_graphics
