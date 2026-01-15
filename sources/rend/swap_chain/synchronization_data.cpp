#pragma once

#include "synchronization_data.hpp"

#include "../swap_chain/swap_chain.hpp"

namespace ars_graphics
{
SynchronizationData::SynchronizationData(const LogicalDevice& device)
{
    vk::FenceCreateInfo fenceInfo{};
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    vk::SemaphoreCreateInfo semaphoreInfo{};

    in_flight_fence = device.get().createFenceUnique(fenceInfo).value;
    image_available = device.get().createSemaphoreUnique(semaphoreInfo).value;
    render_finished = device.get().createSemaphoreUnique(semaphoreInfo).value;
}

const vk::Semaphore&
SynchronizationData::getRenderFinished() const
{
    return render_finished.get();
}

void
SynchronizationData::waitForFence(const LogicalDevice& device) const
{
    device.get().waitForFences(1, &(in_flight_fence.get()), VK_TRUE,
                               UINT64_MAX);

    device.get().resetFences(1, &(in_flight_fence.get()));
}

uint32_t
SynchronizationData::acquireNextImage(const LogicalDevice& device,
                                      const SwapChain& swapchain) const
{
    auto acquire_res = device.get().acquireNextImageKHR(
        swapchain.get(), UINT64_MAX, image_available.get(), nullptr);

    return acquire_res.value;
}

void
SynchronizationData::submit(const LogicalDevice& device,
                            const vk::CommandBuffer& cmd) const
{
    vk::SubmitInfo submitInfo = {};

    vk::PipelineStageFlags waitStages[] = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput};

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores    = &image_available.get();
    submitInfo.pWaitDstStageMask  = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &(cmd);

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = &render_finished.get();

    device.getQueue("graphics").submit(submitInfo, in_flight_fence.get());
}
} // namespace ars_graphics
