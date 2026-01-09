#pragma once
#include <iostream>
#include <vector>

#include "../window/interface_window.hpp"
#include "device/logical_device.hpp"
#include "device/physical_device.hpp"
#include "instance/instance.hpp"
#include "model/model_manager.hpp"
#include "pipelines/pipeline_layout_storage.hpp"
#include "swap_chain/swap_chain.hpp"

#include "scene.hpp"

namespace ars_graphics
{
struct RendererConfigInfo final
{
    IWindow* interface_window;
    std::string render_name;
    std::vector<std::string> models_paths;
};

class RendererImpl final
{
public:
    RendererImpl(const RendererConfigInfo& config_info);

    ~RendererImpl()
    {
        std::cout << "Destructor\n";
    }

    void clear();

private:
    Instance m_instance;

    vk::SurfaceKHR m_surface;

    PhysicalDevice m_physical_device;

    LogicalDevice m_logical_device;

    DescriptorManager m_descriptor_manager;

    PipelineLayoutStorage m_pipeline_layout_storage;

    ModelManager m_model_manager;

    SwapChain m_swapchain;
};

class Renderer final
{
public:
    Renderer(const RendererConfigInfo& config_info)
        : m_renderer_impl(new RendererImpl(config_info))
    {
    }

    // void bind(const Scene& scene)
    // {
    //     m_scene = &scene;
    // }

    ~Renderer()
    {
        std::cout << "Clear\n";
        m_renderer_impl->clear();
        delete m_renderer_impl;
    }

private:
    // const Scene* m_scene;

    RendererImpl* m_renderer_impl = nullptr;
};

}; // namespace ars_graphics
