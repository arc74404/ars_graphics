#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "../shaders/shader_manager.hpp"
#include "../vertex/vertex_types.hpp"

namespace ars_graphics
{

template <typename VertexType>
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
calculateVertexShaderType<vertex_types::PositionOnly3D>()
{
    return ShaderType::DEFAULT_POSITIONAL_ONLY_3D_VERTEX;
}
template <>
inline ShaderType
calculateVertexShaderType<vertex_types::Simple3D>()
{
    return ShaderType::DEFAULT_SIMPLE_3D_VERTEX;
}
template <>
inline ShaderType
calculateVertexShaderType<vertex_types::Standart3D>()
{
    return ShaderType::DEFAULT_STANDART_3D_VERTEX;
}
template <>
inline ShaderType
calculateVertexShaderType<vertex_types::Colored3D>()
{
    return ShaderType::DEFAULT_COLORED_3D_VERTEX;
}
template <>
inline ShaderType
calculateVertexShaderType<vertex_types::ParticleMesh>()
{
    return ShaderType::DEFAULT_PARTICLE_MESH_VERTEX;
}

} // namespace ars_graphics
