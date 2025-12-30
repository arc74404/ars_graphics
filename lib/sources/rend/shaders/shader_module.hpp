#pragma once
#include <fstream>
#include <string_view>

#include "../device/logical_device.hpp"

namespace ars_graphics
{
class ShaderModule final
{
public:
    ShaderModule(const LogicalDevice& device, std::fstream& file);

    const vk::ShaderModule& get() const;

private:
    vk::UniqueShaderModule m_shader_module;
};
} // namespace ars_graphics
