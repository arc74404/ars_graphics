#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "../device/logical_device.hpp"

namespace ars_graphics
{

class Image;

class Buffer
{
public:
    // ---------- //
    static void copyBuffer(const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device,
                           const Buffer* const src_buffer,
                           Buffer* const dst_buffer,
                           vk::Queue queue,
                           const vk::CommandBuffer& command_buffer);

    static void copyBufferToImage(const LogicalDevice& logical_device,
                                  const PhysicalDevice& physical_device,
                                  const Buffer* src_buffer,
                                  Image& dst_image,
                                  vk::Queue queue,
                                  const vk::CommandBuffer& command_buffer,
                                  float w,
                                  float h);

    // ---------- //
    Buffer(vk::BufferUsageFlags buffer_usage_flags,
           vk::MemoryPropertyFlags requested_properties);

    void recreate(const LogicalDevice& logical_device,
                  const PhysicalDevice& physical_device,
                  const vk::DeviceSize& size);

    void checkBufferSize(const LogicalDevice& logical_device,
                         const PhysicalDevice& physical_device,
                         const vk::DeviceSize& required_size);

    virtual bool setData(const LogicalDevice& logical_device,
                         const PhysicalDevice& physical_device,
                         const void* data,
                         const vk::DeviceSize& byte_size);

    const vk::Buffer& buffer() const noexcept;

    vk::DeviceSize byteSize() const noexcept;

private:
    void allocateBufferMemory(const LogicalDevice& logical_device,
                              const PhysicalDevice& physical_device);

    vk::UniqueBuffer m_buffer;

    vk::DeviceSize m_byte_size;

    vk::BufferUsageFlags m_buffer_usage_flags;
    vk::MemoryPropertyFlags m_requested_properties;

    vk::UniqueDeviceMemory m_memory;
};
} // namespace ars_graphics
