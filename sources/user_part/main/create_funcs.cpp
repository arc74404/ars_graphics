#include "create_funcs.hpp"

std::vector<ars_graphics::RenderPassUserConfigInfo>
ars_graphics::createRpConfigs()
{
    RenderPassUserConfigInfo first;
    first.clear_color = true;
    first.clear_depth = true;
    return std::vector<RenderPassUserConfigInfo>{first};
}

std::vector<ars_graphics::ShaderConfigInfo>
ars_graphics::createShConfigs()
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
    return shaders;
}
