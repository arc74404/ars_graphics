#pragma once

#include "../device/logical_device.hpp"

namespace ars_graphics
{
struct SynchronizationData final
{
public:
    SynchronizationData(vk::Device device);
    void destroy();
    vk::UniqueFence m_in_flight_fence;
    vk::UniqueSemaphore m_image_available;
    vk::UniqueSemaphore m_render_finished;
};

} // namespace ars_graphics
