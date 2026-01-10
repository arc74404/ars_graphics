#pragma once
#include <iostream>
#include <vector>

#include "../window/interface_window.hpp"
#include "device/logical_device.hpp"
#include "device/physical_device.hpp"
#include "instance/instance.hpp"
#include "model/model_manager.hpp"
#include "pipelines/pipeline_layout_storage.hpp"
#include "pipelines/pipeline_manager.hpp"
#include "swap_chain/swap_chain.hpp"

#include "scene.hpp"

namespace ars_graphics
{
struct RendererConfigInfo final
{
    IWindow* interface_window;
    std::string render_name;
    std::unordered_map<ShaderType, std::string> shader_paths;
    std::vector<std::string> models_paths;
};

struct Formats
{
    Formats(vk::Format depth_format, vk::SurfaceFormatKHR surface_format)
        : m_depth_format(depth_format), m_surface_format(surface_format)
    {
    }
    vk::Format m_depth_format;
    vk::SurfaceFormatKHR m_surface_format;
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

    Formats m_formats;

    RenderPassManager m_renderpass_manager;

    SwapChain m_swapchain;

    ShaderManager m_shader_manager;

    PipelineManager m_pipeline_manager;

    ModelManager m_model_manager;
};

class Renderer final
{
public:
    Renderer(const RendererConfigInfo& config_info)
    {
        try
        {
            m_renderer_impl = new RendererImpl(config_info);
        }
        catch (std::logic_error& error)
        {
            std::cout << error.what();
            m_is_valid = false;
        }
        catch (std::runtime_error& error)
        {
            std::cout << error.what();
            m_is_valid = false;
        }
    }
    // void bind(const Scene& scene)
    // {
    //     m_scene = &scene;
    // }
    bool IsValid() const
    {
        return m_is_valid;
    }

    ~Renderer()
    {
        if (m_is_valid == true)
        {
            std::cout << "Clear\n";
            m_renderer_impl->clear();
            delete m_renderer_impl;
        }
    }

private:
    // const Scene* m_scene;

    bool m_is_valid = true;

    RendererImpl* m_renderer_impl = nullptr;
};

}; // namespace ars_graphics
