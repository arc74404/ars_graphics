#pragma once

#include <optional>

#include "../device/logical_device.hpp"
#include "../libs_includes/vulkan.hpp"

namespace ars_graphics
{

class Device;

class CommandPool final
{
public:
    CommandPool(vk::Device logical_device,
                const PhysicalDevice& physical_device);

    std::optional<vk::UniqueCommandBuffer> allocateCommandBuffer(
        vk::Device logical_device) const;

    void destroy();

private:
    vk::UniqueCommandPool m_command_pool;
};

} // namespace ars_graphics
