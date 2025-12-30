#pragma once

#include "../rend/renderer.hpp"

namespace ars_graphics
{

struct WindowConfigInfo final
{
};

class Window
{
public:
    Window(const WindowConfigInfo& config_info);

private:
    WindowConfigInfo m_config_info;

    Renderer m_renderer;
};
} // namespace ars_graphics
