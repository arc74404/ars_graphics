#pragma once
#include <iostream>
#include <vector>

#include "../camera/interface_camera.hpp"
#include "../device/logical_device.hpp"
#include "../device/physical_device.hpp"
#include "../instance/instance.hpp"
#include "../model/model_manager.hpp"
#include "../pipelines/pipeline_layout_storage.hpp"
#include "../pipelines/pipeline_manager.hpp"
#include "../swap_chain/swap_chain.hpp"

#include "render_info.hpp"
#include "ubo_data.hpp"

namespace ars_graphics
{

struct IWindow;

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

struct RenderingInfo
{
    ICamera& camera;

    vk::Extent2D window_size;
};

class RendererImpl final
{
public:
    RendererImpl(const RendererConfigInfo& config_info);

    ~RendererImpl()
    {
        clear();
        std::cout << "Destructor\n";
    }

    template <typename T>
    void bind(const T& obj)
    {
        m_render_info_data =
            obj.calculateRenderInfo(m_logical_device, m_physical_device);
    }

    void render(const RenderingInfo& extra_rendering_info);

    void clear();

    const Model* getModel(const std::string& name);

private:
    void draw(uint8_t index);

    void recreate(const vk::Extent2D& window_size);

    void updateUniformBuffer(const SwapChainFrame& frame,
                             const RenderingInfo& rendering_info);

    void startRenderPass(RenderPassType renderpass_type, uint32_t image_index);

    void setupScope();

    vk::Result submit(uint32_t image_index);

    vk::Result present(uint32_t image_index);

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

    ModelManager m_model_manager;
    ///

    RenderInfo m_render_info_data;

    RenderCtx m_render_ctx;

    ///

    VertexShaderUbo m_vertex_shader_ubo;

    // synchronization

    void waitFence(uint8_t index);

    vk::ResultValue<uint32_t> acquireNextImage(uint8_t index);

    uint32_t frame_number = 0;

    struct Sync
    {
        Sync(const LogicalDevice& device, size_t count)
        {
            for (int i = 0; i < count; ++i)
            {
                m_data.emplace_back(device);
            }
        }
        std::vector<SynchronizationData> m_data;
    };

    Sync m_sync;
};

class Renderer final
{
public:
    Renderer(const RendererConfigInfo& config_info)
    {
        m_renderer_impl =
            static_cast<RendererImpl*>(std::malloc(sizeof(RendererImpl)));
        try
        {
            m_renderer_impl = new (m_renderer_impl) RendererImpl(config_info);
        }
        catch (std::logic_error& error)
        {
            std::cout << error.what();

            std::free(m_renderer_impl);
            m_renderer_impl = nullptr;
            m_is_valid      = false;
        }
        catch (std::runtime_error& error)
        {
            std::cout << error.what();

            std::free(m_renderer_impl);
            m_renderer_impl = nullptr;
            m_is_valid      = false;
        }
    }

    const Model* getModel(const std::string& name)
    {
        return m_renderer_impl->getModel(name);
    }

    template <typename T>
    void bind(const T& obj)
    {
        m_renderer_impl->bind(obj);
    }

    void clear()
    {
        m_is_valid = false;
        m_renderer_impl->~RendererImpl();
        std::free(m_renderer_impl);
    }

    void render(const RenderingInfo& extra_rendering_info)
    {
        m_renderer_impl->render(extra_rendering_info);
    }

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
    bool m_is_valid = true;

    RendererImpl* m_renderer_impl = nullptr;
};

}; // namespace ars_graphics
