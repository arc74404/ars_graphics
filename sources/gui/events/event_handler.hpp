#pragma once

#include "../../rend/camera/interface_camera.hpp"
#include "../../rend/render/renderer.hpp"
#include "../window/interface_window.hpp"

namespace ars_graphics
{

struct EventHandlerContext
{
    IWindow& window;
    ICamera& camera;
    Renderer& renderer;

    double delta_time;
};

class EventHandler final
{
public:
    void handle(const EventHandlerContext& ctx,
                const std::vector<EventPtr>& events);

private:
};
} // namespace ars_graphics
