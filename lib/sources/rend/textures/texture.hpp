#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <string_view>

#include "../descriptors/descriptor_manager.hpp"
#include "../device/logical_device.hpp"

#include "image.hpp"

namespace ars_graphics
{
class Texture
{
public:
    Texture(const LogicalDevice& logical_device,
            const PhysicalDevice& physical_device);

    void loadTexture(const LogicalDevice& device,
                     const PhysicalDevice& physical_device,
                     std::string_view file_path);

private:
    Image m_image;
    vk::UniqueSampler m_sampler;
};
} // namespace ars_graphics
