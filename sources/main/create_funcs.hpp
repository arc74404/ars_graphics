#pragma once
#include <vector>

#include "../rend/render_pass/render_pass_config_info.hpp"
#include "../rend/shaders/shader_config_info.hpp"

namespace ars_graphics
{
std::vector<RenderPassUserConfigInfo>
createRpConfigs()
{
    RenderPassUserConfigInfo first;
    first.clear_color = true;
    first.clear_depth = true;
    return {first};
}

std::vector<ShaderConfigInfo>
createShConfigs()
{
    std::string folder =
        "C:/Users/User/source/repos/arsrender_lib/shaders/compiled/";
    std::vector<ShaderConfigInfo> shaders = {
        {folder + "vert/model2d/DEFAULT_SIMPLE_2D_VERTEX.vert.spv",
         ShaderType::VERTEX                                                                      },
        {folder + "vert/model3d/DEFAULT_COLORED_3D_VERTEX.vert.spv",
         ShaderType::VERTEX                                                                      },
        {folder + "vert/model3d/DEFAULT_PARTICLE_MESH_VERTEX.vert.spv",
         ShaderType::VERTEX                                                                      },
        {folder + "vert/model3d/DEFAULT_POSITIONAL_ONLY_3D_VERTEX.vert.spv",
         ShaderType::VERTEX                                                                      },
        {folder + "vert/model3d/DEFAULT_SIMPLE_3D_VERTEX.vert.spv",
         ShaderType::VERTEX                                                                      },
        {folder + "vert/model3d/DEFAULT_STANDART_3D_VERTEX.vert.spv",
         ShaderType::VERTEX                                                                      },
        {folder + "frag/DEFAULT_FRAGMENT.frag.spv",                          ShaderType::FRAGMENT},
        {folder + "vert/model2d/DEFAULT_SIMPLE_2D_VERTEX.vert.spv",
         ShaderType::VERTEX                                                                      }
    };
}
} // namespace ars_graphics
