#include <iostream>
#include <vector>

#include "../gui/window/glfw_window.hpp"

#include "create_funcs.hpp"

using namespace ars_graphics;
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
                                .surface_creater     = window,
                                .width               = window.getWidth(),
                                .height              = window.getHeight()});

    std::unique_ptr<RenderPipeline> pipeline = createRenderPipeline();

    ResourceScene scene;

    RenderContext ctx{.render_pass_index = 0};

    while (window.IsOpen())
    {
        // std::cout << "render\n";
        render(ctx, setuper, *pipeline, scene);

        // std::cout << "pollEvents\n";
        // auto&& events = window.pollEvents();

        // std::cout << "handle\n";

        // std::cout << delta_time << '\n';
    }

    return 0;
}
