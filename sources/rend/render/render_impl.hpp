#pragma once

#include "../../libs_includes/vulkan.hpp"
#include "../cmdbuf/command_pool.hpp"
#include "../device/logical_device.hpp"
#include "../device/physical_device.hpp"
#include "../gui/window/interface_window.hpp"
#include "../instance/instance.hpp"
#include "../scene/interface_scene.hpp"
#include "../swap_chain/swap_chain.hpp"

#include "synchronization_data.hpp"

namespace ars_graphics
{

struct RenderConfigInfo
{
    RenderConfigInfo();

    std::string inst_name;

    std::vector<RenderPassConfigInfo> render_pass_configs;

    IWindow& window;
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
    RendererImpl(const RenderConfigInfo& render_info);

    ~RendererImpl()
    {
        clear();
    }

    void bindScene(IScene& scene);

    void render();

    void clear();

private:
    // pipeline for bind
    // pipeline for render

    virtual void updatePerFrameBuffers() = 0;

    //
    void draw(uint8_t index);

    void recreate(const vk::Extent2D& window_size);

    void startRenderPass(uint32_t image_index);

    void setupScope();

    vk::Result submit(uint32_t image_index);

    vk::Result present(uint32_t image_index);

    Instance m_instance;

    vk::SurfaceKHR m_surface;

    PhysicalDevice m_physical_device;

    LogicalDevice m_logical_device;

    std::vector<vk::UniqueRenderPass> m_render_passes;
    std::vector<vk::UniqueRenderPass> createRenderPasses(
        const std::vector<RenderPassConfigInfo>& render_pass_configs) const;

    CommandPool m_command_pool;

    Formats m_formats;

    SwapChain m_swapchain;

    // TODO: make in an other place

    std::vector<SwapChainFrame> createFrames(vk::RenderPass render_pass) const;
    std::vector<std::vector<SwapChainFrame>> createAllFrames() const;
    std::vector<std::vector<SwapChainFrame>>
        m_frames; // n frames on 3 different render passes

    vk::UniqueFramebuffer createFramebuffer(vk::RenderPass render_pass,
                                            vk::ImageView view,
                                            vk::ImageView depth_view) const;

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

} // namespace ars_graphics
