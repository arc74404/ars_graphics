#pragma once

#include "../device/logical_device.hpp"

namespace ars_graphics
{
//
// class SwapChain;

// class SynchronizationData final
// {
// public:
//     SynchronizationData(const LogicalDevice& device);

//     vk::Result waitForFence(const LogicalDevice& device) const;

//     vk::ResultValue<uint32_t> acquireNextImage(
//         const LogicalDevice& device,
//         const SwapChain& swapchain) const;

//     vk::Result submit(const LogicalDevice& device,
//                       const vk::CommandBuffer& cmd) const;

//     const vk::Semaphore& getRenderFinished() const;

//     void destroy();

// private:
//     vk::UniqueFence m_in_flight_fence;
//     vk::UniqueSemaphore m_image_available;
//     vk::UniqueSemaphore m_render_finished;
// };

struct SynchronizationData final
{
public:
    SynchronizationData(const LogicalDevice& device);
    void destroy();
    vk::UniqueFence m_in_flight_fence;
    vk::UniqueSemaphore m_image_available;
    vk::UniqueSemaphore m_render_finished;
};

} // namespace ars_graphics
