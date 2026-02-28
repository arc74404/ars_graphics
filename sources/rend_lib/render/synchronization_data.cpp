#pragma once
#include "synchronization_data.hpp"

namespace ars_graphics
{
SynchronizationData::SynchronizationData(vk::Device device)
{
    vk::FenceCreateInfo fenceInfo{};
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    vk::SemaphoreCreateInfo semaphoreInfo{};

    m_in_flight_fence = device.createFenceUnique(fenceInfo).value;
    m_image_available = device.createSemaphoreUnique(semaphoreInfo).value;
    m_render_finished = device.createSemaphoreUnique(semaphoreInfo).value;
}

// const vk::Semaphore&
// SynchronizationData::getRenderFinished() const
// {
//     return m_render_finished.get();
// }

// vk::ResultValue<uint32_t>
// SynchronizationData::acquireNextImage(const LogicalDevice& device,
//                                       const SwapChain& swapchain) const
// {
//     return device.get().acquireNextImageKHR(swapchain.get(), UINT64_MAX,
//                                             m_image_available.get(),
//                                             nullptr);
// }

// vk::Result
// SynchronizationData::waitForFence(const LogicalDevice& device) const
// {
//     vk::Result res = device.get().waitForFences(1,
//     &(m_in_flight_fence.get()),
//                                                 VK_TRUE, UINT64_MAX);
//     if (res != vk::Result::eSuccess)
//     {
//         return res;
//     }

//     res = device.get().resetFences(1, &(m_in_flight_fence.get()));

//     if (res != vk::Result::eSuccess)
//     {
//         return res;
//     }
//     return vk::Result::eSuccess;
// }

// vk::Result
// SynchronizationData::submit(const LogicalDevice& device,
//                             const vk::CommandBuffer& cmd) const
// {
//     vk::SubmitInfo submitInfo = {};

//     vk::PipelineStageFlags waitStages[] = {
//         vk::PipelineStageFlagBits::eColorAttachmentOutput};

//     submitInfo.waitSemaphoreCount = 1;
//     submitInfo.pWaitSemaphores    = &m_image_available.get();
//     submitInfo.pWaitDstStageMask  = waitStages;

//     submitInfo.commandBufferCount = 1;
//     submitInfo.pCommandBuffers    = &(cmd);

//     submitInfo.signalSemaphoreCount = 1;
//     submitInfo.pSignalSemaphores    = &m_render_finished.get();

//     return device.getQueue("graphics")
//         .submit(submitInfo, m_in_flight_fence.get());
// }

void
SynchronizationData::destroy()
{
    m_image_available.reset();

    m_render_finished.reset();

    m_in_flight_fence.reset();
}

} // namespace ars_graphics
