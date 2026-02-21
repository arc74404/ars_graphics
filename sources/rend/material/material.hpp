#pragma once

#include <iostream>
#include <string>

#include "../buffers/uniform_buffer.hpp"
#include "../descriptors/descriptor_manager.hpp"
#include "../textures/texture.hpp"
#include "../vertex/vertex.hpp"

#include "interface_material_settuper.hpp"

namespace ars_graphics
{

class NonUpdatebleMaterial final
{
public:
    ~NonUpdatebleMaterial() noexcept;

    NonUpdatebleMaterial(const InterfaceMaterialSettupper& settuper);

    void bind(vk::CommandBuffer cmd, vk::PipelineLayout layout) const;

private:
    std::vector<vk::UniqueDescriptorSet> m_sets_to_bind;
    std::vector<vk::DescriptorSet> m_proxy_to_bind;
};
} // namespace ars_graphics
