#pragma once

#include <iostream>

#include "../../libs_includes/vulkan.hpp"
#include "../device/logical_device.hpp"

namespace ars_graphics
{
class Image;

class Buffer
{
public:
    Buffer(Buffer&& other) noexcept            = default;
    Buffer& operator=(Buffer&& other) noexcept = default;
    virtual ~Buffer() noexcept                 = default;
    // ---------- //

    static vk::Result copyBuffer(const LogicalDevice& logical_device,
                                 const PhysicalDevice& physical_device,
                                 const Buffer& src_buffer,
                                 Buffer& dst_buffer,
                                 vk::Queue queue,
                                 vk::CommandBuffer command_buffer);

    static vk::Result copyBufferToImage(const LogicalDevice& logical_device,
                                        const PhysicalDevice& physical_device,
                                        const Buffer& src_buffer,
                                        Image& dst_image,
                                        vk::Queue queue,
                                        vk::CommandBuffer command_buffer,
                                        uint32_t w,
                                        uint32_t h);
    // const //

    // vk::Buffer get() const noexcept;

    // vk::DeviceSize getByteSize() const noexcept;

    // ---------- //

    Buffer(vk::BufferUsageFlags buffer_usage_flags,
           vk::MemoryPropertyFlags requested_properties);

    vk::Result setData(const LogicalDevice& logical_device,
                       const PhysicalDevice& physical_device,
                       const void* data,
                       vk::DeviceSize byte_size);

protected:
    virtual vk::Result setDataImpl(const LogicalDevice& logical_device,
                                   const PhysicalDevice& physical_device,
                                   const void* data,
                                   vk::DeviceSize byte_size);
    const vk::Buffer& get() const noexcept;

    vk::DeviceSize getByteSize() const noexcept;

private:
    vk::Result recreate(const LogicalDevice& logical_device,
                        const PhysicalDevice& physical_device,
                        vk::DeviceSize size);
    vk::Result allocateBufferMemory(const LogicalDevice& logical_device,
                                    const PhysicalDevice& physical_device);
    vk::Result checkBufferSize(const LogicalDevice& logical_device,
                               const PhysicalDevice& physical_device,
                               vk::DeviceSize required_size);

    void unmap(const LogicalDevice& logical_device) noexcept;

    vk::UniqueBuffer m_buffer;

    vk::DeviceSize m_byte_size;

    vk::BufferUsageFlags m_buffer_usage_flags;
    vk::MemoryPropertyFlags m_requested_properties;

    bool m_is_mapped = false;
    vk::UniqueDeviceMemory m_memory;

    void* m_mapped_memory = nullptr;
};
} // namespace ars_graphics
