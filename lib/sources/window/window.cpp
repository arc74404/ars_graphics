#include "window.hpp"

namespace ars_graphics
{
Window::Window(const WindowConfigInfo& config_info)
    : m_config_info(config_info), m_renderer({})
{
}
} // namespace ars_graphics
