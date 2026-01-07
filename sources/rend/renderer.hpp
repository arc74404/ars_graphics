#pragma once
#include <iostream>
#include <vector>

#include "../window/interface_window.hpp"
#include "device/logical_device.hpp"
#include "device/physical_device.hpp"
#include "instance/instance.hpp"
#include "model/model_manager.hpp"

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

    vk::SurfaceKHR m_surface;

    PhysicalDevice m_physical_device;

    LogicalDevice m_logical_device;

    // ---- models ---- //

    ModelManager m_model_manager;

    // -----------------//
};
}; // namespace ars_graphics
