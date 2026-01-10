#include "../sources/rend/renderer.hpp"
#include "../sources/window/glfw_window.hpp"

using namespace ars_graphics;

int
main()
{
    std::vector<std::string> models_paths = {
        "C:/Users/User/source/repos/arsrender_lib/models/"
        "AntiqueCamera.glb",
        "C:/Users/User/source/repos/arsrender_lib/models/"
        "BrainStem.glb"};

    std::string folder =
        "C:/Users/User/source/repos/arsrender_lib/shaders/compiled/";

    std::unordered_map<ars_graphics::ShaderType, std::string> shader_paths = {
        {ars_graphics::ShaderType::DEFAULT_SIMPLE_2D_VERTEX,
         folder + "DEFAULT_SIMPLE_2D_VERTEX.vert.spv"         },
        {ars_graphics::ShaderType::DEFAULT_COLORED_3D_VERTEX,
         folder + "DEFAULT_COLORED_3D_VERTEX.vert.spv"        },
        {ars_graphics::ShaderType::DEFAULT_PARTICLE_MESH_VERTEX,
         folder + "DEFAULT_PARTICLE_MESH_VERTEX.vert.spv"     },
        {ars_graphics::ShaderType::DEFAULT_POSITIONAL_ONLY_3D_VERTEX,
         folder + "DEFAULT_POSITIONAL_ONLY_3D_VERTEX.vert.spv"},
        {ars_graphics::ShaderType::DEFAULT_SIMPLE_3D_VERTEX,
         folder + "DEFAULT_SIMPLE_3D_VERTEX.vert.spv"         },
        {ars_graphics::ShaderType::DEFAULT_STANDART_3D_VERTEX,
         folder + "DEFAULT_STANDART_3D_VERTEX.vert.spv"       },
        {ars_graphics::ShaderType::DEFAULT_FRAGMENT,
         folder + "DEFAULT_FRAGMENT.frag.spv"                 },
    };

    GlfwWindow window({{.m_title = "titlew"}});

    RendererConfigInfo config_info = {.interface_window = &window,
                                      .render_name      = "checker",
                                      .shader_paths     = shader_paths,
                                      .models_paths     = models_paths};

    Renderer renderer(config_info);

    if (false == renderer.IsValid())
    {
        return 1;
    }

    // Scene scene;

    // renderer.bind(scene);

    // while (true)
    // {
    // }

    return 0;
}
