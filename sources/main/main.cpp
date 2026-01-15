#include "../sources/gui/window/glfw_window.hpp"
#include "../sources/rend/render/renderer.hpp"

using namespace ars_graphics;

#include <iostream>
#include <vector>

#include "../sources/rend/scene.hpp"

int
main()
{
    std::vector<std::string> models_paths = {
        "C:/Users/User/source/repos/arsrender_lib/models/"
        "Box.glb"};

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
                                      .shader_paths     = shader_paths};

    Renderer renderer(config_info);

    ModelManager model_manager = renderer.generateModelManager(models_paths);

    if (false == renderer.IsValid())
    {
        return 1;
    }

    Scene scene;
    scene.addModel(
        model_manager["C:/Users/User/source/repos/arsrender_lib/models/"
                      "Box.glb"]);

    renderer.bind(scene);

    while (true)
    {
        renderer.render();
        if (EventType::CLOSE == window.pollEvents())
        {
            return 1;
        }
    }

    return 0;
}
