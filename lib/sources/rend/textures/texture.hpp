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

    int getWidth() const noexcept;
    int getHeight() const noexcept;

    void loadTexture(const LogicalDevice& device,
                     const PhysicalDevice& physical_device,
                     std::string_view file_path);

    void allocDescriptorSet(const LogicalDevice& logical_device,
                            const DescriptorManager& desc_manager);

    void bind(const vk::CommandBuffer& command_buffer,
              const vk::PipelineLayout& pipelayout) const;

private:
    int m_width;
    int m_height;

    Image m_image;

    vk::UniqueSampler m_sampler;

    vk::UniqueDescriptorSet m_descriptor_set;
};
} // namespace ars_graphics