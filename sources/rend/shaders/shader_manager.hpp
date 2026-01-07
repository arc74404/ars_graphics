#ifndef SHADER_HPP
#define SHADER_HPP

#include <vulkan/vulkan.hpp>

#include <string_view>
#include <unordered_map>

#include "shader_module.hpp"

namespace ars_graphics
{
enum class ShaderType
{
    DEFAULT_2D_VERTEX,
    DEFAULT_3D_VERTEX,
    DEFAULT_FRAGMENT
};

class ShaderManager final
{
public:
    ShaderManager(const LogicalDevice& device,
                  const std::string& shader_folder_path);

    const ShaderModule& operator[](ShaderType st) const;

private:
    std::map<ShaderType, ShaderModule> m_modules;
};

}; // namespace ars_graphics

#endif // SHADER_HPP
