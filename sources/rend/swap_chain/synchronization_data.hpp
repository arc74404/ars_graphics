#pragma once

#include "../device/logical_device.hpp"

namespace ars_graphics
{

class SwapChain;

class SynchronizationData final
{
public:
    SynchronizationData(const LogicalDevice& device);

    void waitForFence(const LogicalDevice& device) const;

    uint32_t acquireNextImage(const LogicalDevice& device,
                              const SwapChain& swapchain) const;

    void submit(const LogicalDevice& device,
                const vk::CommandBuffer& cmd) const;

    const vk::Semaphore& getRenderFinished() const;

private:
    vk::UniqueFence in_flight_fence;
    vk::UniqueSemaphore image_available;
    vk::UniqueSemaphore render_finished;
};

} // namespace ars_graphics
