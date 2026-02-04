#include "../gui/events/event_handler.hpp"
#include "../sources/gui/window/glfw_window.hpp"
#include "../sources/rend/render/renderer.hpp"

using namespace ars_graphics;

#include <iostream>
#include <vector>

#include "../rend/camera/fly_camera.hpp"
#include "../sources/rend/scene.hpp"
#include "../sources/time/time_manager.hpp"

#define MODEL_GOPHER_NAME \
    "C:/Users/User/source/repos/arsrender_lib/models/go_gopher.glb"
#define CAMERA_NAME \
    "C:/Users/User/source/repos/arsrender_lib/models/AntiqueCamera.glb"

#define MODEL_NAME MODEL_GOPHER_NAME

int
main()
{
    std::vector<std::string> models_paths = {MODEL_NAME};

    std::string folder =
        "C:/Users/User/source/repos/arsrender_lib/shaders/compiled/";

    std::unordered_map<ars_graphics::ShaderType, std::string> shader_paths = {
        {ars_graphics::ShaderType::DEFAULT_SIMPLE_2D_VERTEX,
         folder + "vert/model2d/DEFAULT_SIMPLE_2D_VERTEX.vert.spv"         },
        {ars_graphics::ShaderType::DEFAULT_COLORED_3D_VERTEX,
         folder + "vert/model3d/DEFAULT_COLORED_3D_VERTEX.vert.spv"        },
        {ars_graphics::ShaderType::DEFAULT_PARTICLE_MESH_VERTEX,
         folder + "vert/model3d/DEFAULT_PARTICLE_MESH_VERTEX.vert.spv"     },
        {ars_graphics::ShaderType::DEFAULT_POSITIONAL_ONLY_3D_VERTEX,
         folder + "vert/model3d/DEFAULT_POSITIONAL_ONLY_3D_VERTEX.vert.spv"},
        {ars_graphics::ShaderType::DEFAULT_SIMPLE_3D_VERTEX,
         folder + "vert/model3d/DEFAULT_SIMPLE_3D_VERTEX.vert.spv"         },
        {ars_graphics::ShaderType::DEFAULT_STANDART_3D_VERTEX,
         folder + "vert/model3d/DEFAULT_STANDART_3D_VERTEX.vert.spv"       },
        {ars_graphics::ShaderType::DEFAULT_FRAGMENT,
         folder + "frag/DEFAULT_FRAGMENT.frag.spv"                         },
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

    Scene scene;

    scene.addModel(renderer.getModel(MODEL_NAME));

    renderer.bind(scene);

    FlyCamera fly_camera(window.getWidth(), window.getHeight());

    EventHandler event_handler;

    TimeManager time_manager;

    while (window.IsOpen())
    {
        double delta_time = time_manager.restartTimer();

        // std::cout << "render\n";
        renderer.render({.camera = fly_camera});

        // std::cout << "pollEvents\n";
        auto&& events = window.pollEvents();

        // std::cout << "handle\n";

        event_handler.handle({.window     = window,
                              .camera     = fly_camera,
                              .renderer   = renderer,
                              .delta_time = delta_time},
                             events);

        // std::cout << delta_time << '\n';
    }

    return 0;
}
