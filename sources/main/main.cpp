#include "../gui/events/event_handler.hpp"
#include "../rend/render/render_core.hpp"
#include "../rend/render/render_pipeline.hpp"
#include "../rend/render/render_setuper.hpp"
#include "../sources/gui/window/glfw_window.hpp"

#include "create_funcs.hpp"

using namespace ars_graphics;

#include <iostream>
#include <vector>

#include "../rend/camera/fly_camera.hpp"
#include "../sources/time/time_manager.hpp"

#define MODEL_GOPHER_NAME \
    "C:/Users/User/source/repos/arsrender_lib/models/gopher.glb"
#define CAMERA_NAME \
    "C:/Users/User/source/repos/arsrender_lib/models/AntiqueCamera.glb"

#define MODEL_NAME MODEL_GOPHER_NAME

int
main()
{
    std::vector<std::string> models_paths = {MODEL_NAME, CAMERA_NAME};

    GlfwWindow window({{.m_title = "titlew"}});

    std::vector<RenderPassUserConfigInfo> render_pass_configs =
        createRpConfigs();

    std::vector<ShaderConfigInfo> shaders_config_info = createShConfigs();

    RenderSetuper setuper(
        RenderSetuperConfigInfo{.inst_name           = "test",
                                .render_pass_configs = render_pass_configs,
                                .shaders_config_info = shaders_config_info,
                                .window              = window});

    RenderPipeline* pipeline;

    ResourceScene scene;

    FlyCamera fly_camera(window.getWidth(), window.getHeight());

    EventHandler event_handler;

    TimeManager time_manager;

    RenderContext ctx;

    while (window.IsOpen())
    {
        double delta_time = time_manager.restartTimer();

        // std::cout << "render\n";
        render(ctx, setuper, *pipeline, scene);

        // std::cout << "pollEvents\n";
        auto&& events = window.pollEvents();

        // std::cout << "handle\n";

        // std::cout << delta_time << '\n';
    }

    return 0;
}
