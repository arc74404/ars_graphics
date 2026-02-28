#include "interface_camera.hpp"

namespace ars_graphics
{
bool
ICamera::needRecalculation() const noexcept
{
    return m_need_recalculation;
}

void
ICamera::processKeyboard(Key key, KeyStatus status, double delta_time)
{
    processKeyboardImpl(key, status, delta_time);
}

void
ICamera::processMouseMovement(const glm::vec2& shift,
                              double delta_time,
                              bool constrain_pitch)
{
    processMouseMovementImpl(shift, delta_time, constrain_pitch);
}

void
ICamera::processMouseScroll(float yoffset)
{
    processMouseScrollImpl(yoffset);
}

glm::mat4
ICamera::recalculate()
{
    return recalculateImpl();
}
} // namespace ars_graphics
