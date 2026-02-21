#pragma once

#include "../device/logical_device.hpp"
#include "../device/physical_device.hpp"

namespace ars_graphics
{
template <typename InstanceDataT, typename BufferT>
class Instancer
{
public:
    BufferT generateBuffer(vk::Device logical_device,
                           const PhysicalDevice& physical_device)
    {
        BufferT buf;
        buf.setData(logical_device, physical_device, m_data.data(),
                    m_data.size() * sizeof(InstanceDataT));
        return buf;
    }

    void push(const InstanceDataT& instance_data)
    {
        m_data.emplace_back(instance_data);
    }

private:
    std::vector<InstanceDataT> m_data;
};
} // namespace ars_graphics
