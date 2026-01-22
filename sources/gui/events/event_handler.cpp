#include "event_handler.hpp"

#include <iostream>

namespace ars_graphics
{
void
EventHandler::handle(const EventHandlerContext& ctx,
                     const std::vector<EventPtr>& events)
{
    for (auto&& event : events)
    {
        switch (event->m_type)
        {
            case Event::Type::CLOSE_WINDOW:
                ctx.renderer.clear();
                ctx.window.close();
                break;

            case Event::Type::KEY:
                ctx.camera.processKeyboard(
                    static_cast<const KeyEvent*>(event.get())->m_key,
                    static_cast<const KeyEvent*>(event.get())->m_status,
                    ctx.delta_time);
                break;

            case Event::Type::MOVE_MOUSE:
                ctx.camera.processMouseMovement(
                    static_cast<const MoveMouseEvent*>(event.get())->m_shift,
                    ctx.delta_time, true);
                break;
            default:
                break;
        }
    }
}
} // namespace ars_graphics
