#pragma once
#include <string>

namespace ars_graphics
{

enum class ShaderType
{
    VERTEX,
    FRAGMENT
};

struct ShaderConfigInfo final
{
    std::string filename;
    ShaderType shader_type;
};
} // namespace ars_graphics
