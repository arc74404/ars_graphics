#pragma once

#include <vulkan/vulkan.hpp>

#include <map>
#include <string_view>
#include <unordered_map>

#include "shader_module.hpp"

namespace ars_graphics
{
enum class ShaderType
{
    DEFAULT_SIMPLE_2D_VERTEX,
    DEFAULT_POSITIONAL_ONLY_3D_VERTEX,
    DEFAULT_SIMPLE_3D_VERTEX,
    DEFAULT_STANDART_3D_VERTEX,
    DEFAULT_COLORED_3D_VERTEX,
    DEFAULT_PARTICLE_MESH_VERTEX,
    DEFAULT_FRAGMENT,
    TOTAL_COUNT
};

class ShaderManager final
{
public:
    ShaderManager(
        const LogicalDevice& device,
        const std::unordered_map<ShaderType, std::string>& shader_paths);

    const ShaderModule& operator[](ShaderType st) const;

private:
    std::map<ShaderType, ShaderModule> m_modules;
};

}; // namespace ars_graphics
