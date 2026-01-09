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

    GlfwWindow window({{.m_title = "titlew"}});

    RendererConfigInfo config_info = {.interface_window = &window,
                                      .render_name      = "checker",
                                      .models_paths     = models_paths};

    Renderer renderer(config_info);

    // Scene scene;

    // renderer.bind(scene);

    // while (true)
    // {
    // }

    return 0;
}
