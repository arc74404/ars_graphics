#pragma once

#include "../../gui/window/interface_window.hpp"
#include "../../libs_includes/vulkan.hpp"
#include "../cmdbuf/command_pool.hpp"
#include "../device/logical_device.hpp"
#include "../device/physical_device.hpp"
#include "../instance/instance.hpp"
#include "../render_pass/render_pass_creater.hpp"
#include "../scene/interface_scene.hpp"
#include "../shaders/shader_config_info.hpp"
#include "../swap_chain/swap_chain.hpp"
#include "../swap_chain/swap_chain_frame.hpp"

#include "synchronization_data.hpp"

namespace ars_graphics
{

struct RenderSetuperConfigInfo
{
    RenderSetuperConfigInfo();

    std::string inst_name;

    std::vector<RenderPassConfigInfo> render_pass_configs; // render passes

    std::vector<ShaderConfigInfo> shaders;

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
struct Sync
{
    Sync(vk::Device device, size_t count)
    {
        for (int i = 0; i < count; ++i)
        {
            m_data.emplace_back(device);
        }
    }
    std::vector<SynchronizationData> m_data;
};

class RenderSetuper final
{
public:
    RenderSetuper(const RenderSetuperConfigInfo& render_info);

    ~RenderSetuper()
    {
        clear();
    }

    /// getters
    vk::RenderPass getRenderPass(uint32_t render_pass_index) const;

    const SwapChainFrame& getFrame(uint32_t frame_index,
                                   uint32_t render_pass_index) const;

    uint32_t getFrameNumber() const;

    const vk::Extent2D& getExtent() const;

    ///
    void clear();

    void waitFence(uint8_t index) const;

    vk::ResultValue<uint32_t> acquireNextImage(uint8_t index);

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

    std::vector<std::vector<SwapChainFrame>> createAllFrames() const;
    std::vector<std::vector<SwapChainFrame>>
        m_frames; // n frames on m different render passes

    // synchronization

    uint32_t m_frame_number = 0;
    Sync m_sync;
};

} // namespace ars_graphics
