#pragma once
#include <iostream>
#include <vector>

#include "../../gui/window/interface_window.hpp"
#include "../device/logical_device.hpp"
#include "../device/physical_device.hpp"
#include "../instance/instance.hpp"
#include "../model/model_manager.hpp"
#include "../pipelines/pipeline_layout_storage.hpp"
#include "../pipelines/pipeline_manager.hpp"
#include "../swap_chain/swap_chain.hpp"

#include "render_info.hpp"

namespace ars_graphics
{
struct RendererConfigInfo final
{
    IWindow* interface_window;
    std::string render_name;
    std::unordered_map<ShaderType, std::string> shader_paths;
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

struct CommandPoolConstructControler
{
    CommandPoolConstructControler(const LogicalDevice& logical_device,
                                  const PhysicalDevice& physical_device)
    {
        CommandBuffer::createCommandPool(logical_device, physical_device);
    }

    void destroy()
    {
        CommandBuffer::destroyCommandPool();
    }
};

class RendererImpl final
{
public:
    RendererImpl(const RendererConfigInfo& config_info);

    ~RendererImpl()
    {
        std::cout << "Destructor\n";
    }

    template <typename T>
    void bind(const T& obj)
    {
        m_render_info_data =
            obj.calculateRenderInfo(m_logical_device, m_physical_device);
    }

    void render();

    void clear();

    ModelManager generateModelManager(
        const std::vector<std::string>& models_paths);

private:
    void startRenderPass(RenderPassType renderpass_type);

    void setupScope();

    void present(const SynchronizationData& sync, uint32_t image_index);

    Instance m_instance;

    vk::SurfaceKHR m_surface;

    PhysicalDevice m_physical_device;

    LogicalDevice m_logical_device;

    CommandPoolConstructControler m_command_pool_controler;

    DescriptorManager m_descriptor_manager;

    Formats m_formats;

    RenderPassManager m_renderpass_manager;

    SwapChain m_swapchain;

    ShaderManager m_shader_manager;

    PipelineManager m_pipeline_manager;
    ///

    RenderInfo m_render_info_data;

    RenderCtx m_render_ctx;
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

    template <typename T>
    void bind(const T& obj)
    {
        m_renderer_impl->bind(obj);
    }

    void render()
    {
        m_renderer_impl->render();
    }

    bool IsValid() const
    {
        return m_is_valid;
    }

    ModelManager generateModelManager(
        const std::vector<std::string>& models_paths)
    {
        return m_renderer_impl->generateModelManager(models_paths);
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
    bool m_is_valid = true;

    RendererImpl* m_renderer_impl = nullptr;
};

}; // namespace ars_graphics
