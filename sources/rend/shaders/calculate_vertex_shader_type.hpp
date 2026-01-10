#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "../mesh/primitives_types.hpp"
#include "../shaders/shader_manager.hpp"

namespace ars_graphics
{

template <typename PrimitiveType>
ShaderType
calculateVertexShaderType()
{
    static_assert(
        false,
        "calculateVertexShaderType not implemented for this vertex type");
    return ShaderType::DEFAULT_COLORED_3D_VERTEX;
}

template <>
inline ShaderType
calculateVertexShaderType<primitive_types::PositionOnly3D>()
{
    return ShaderType::DEFAULT_POSITIONAL_ONLY_3D_VERTEX;
}
template <>
inline ShaderType
calculateVertexShaderType<primitive_types::Simple3D>()
{
    return ShaderType::DEFAULT_SIMPLE_3D_VERTEX;
}
template <>
inline ShaderType
calculateVertexShaderType<primitive_types::Standart3D>()
{
    return ShaderType::DEFAULT_STANDART_3D_VERTEX;
}
template <>
inline ShaderType
calculateVertexShaderType<primitive_types::Colored3D>()
{
    return ShaderType::DEFAULT_COLORED_3D_VERTEX;
}
template <>
inline ShaderType
calculateVertexShaderType<primitive_types::ParticleMesh>()
{
    return ShaderType::DEFAULT_PARTICLE_MESH_VERTEX;
}

} // namespace ars_graphics
