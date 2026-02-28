#pragma once

#include <iostream>

#include "../device/logical_device.hpp"
#include "../libs_includes/vulkan.hpp"

#define FAILED_RESULT_RETURN(command, success_res) \
    {                                              \
        auto&& res = command;                      \
        if (res != success_res)                    \
        {                                          \
            return res;                            \
        }                                          \
    }

#define FAILED_RESULT_VALUE_RETURN(name_res, command, success_res) \
    auto&& name_res = command;                                     \
    if (name_res.result != success_res)                            \
    {                                                              \
        return name_res.result;                                    \
    }

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

    static vk::Result copyBuffer(const Buffer& src_buffer,
                                 Buffer& dst_buffer,
                                 vk::Queue queue,
                                 vk::CommandBuffer command_buffer);

    static vk::Result copyBufferToImage(const Buffer& src_buffer,
                                        Image& dst_image,
                                        vk::Queue queue,
                                        vk::CommandBuffer command_buffer,
                                        uint32_t w,
                                        uint32_t h);
    // const //

    // vk::Buffer get() const noexcept;

    // vk::DeviceSize getByteSize() const noexcept;

    // ---------- //

    Buffer(const LogicalDevice& logical_device,
           const PhysicalDevice& physical_device,
           vk::BufferUsageFlags buffer_usage_flags,
           vk::MemoryPropertyFlags requested_properties);

    vk::Result setData(const void* data, vk::DeviceSize byte_size);

    operator vk::Buffer() const noexcept;

    vk::DeviceSize getByteSize() const noexcept;

protected:
    virtual vk::Result setDataImpl(const void* data, vk::DeviceSize byte_size);
    const LogicalDevice& m_logical_device;
    const PhysicalDevice& m_physical_device;

private:
    vk::Result recreate(vk::DeviceSize size);
    vk::Result allocateBufferMemory();
    vk::Result checkBufferSize(vk::DeviceSize required_size);

    void unmap() noexcept;

    //////////////////

    vk::UniqueBuffer m_buffer;

    vk::DeviceSize m_byte_size;

    vk::BufferUsageFlags m_buffer_usage_flags;
    vk::MemoryPropertyFlags m_requested_properties;

    bool m_is_mapped = false;
    vk::UniqueDeviceMemory m_memory;

    void* m_mapped_memory = nullptr;
};
} // namespace ars_graphics
