#pragma once

#include "../../gui/events/key.hpp"
#include "glm/glm.hpp"

namespace ars_graphics
{
class ICamera
{
public:
    void processKeyboard(Key key, KeyStatus status, double delta_time);

    void processMouseMovement(const glm::vec2& shift,
                              double delta_time,
                              bool constrain_pitch = true);

    void processMouseScroll(float yoffset);

    glm::mat4 recalculate();

    // const //

    bool needRecalculation() const noexcept;

protected:
    virtual glm::mat4 recalculateImpl() = 0;

    virtual void processKeyboardImpl(Key key,
                                     KeyStatus status,
                                     double delta_time) = 0;

    virtual void processMouseMovementImpl(const glm::vec2& shift,
                                          double delta_time,
                                          bool constrain_pitch) = 0;

    virtual void processMouseScrollImpl(float yoffset) = 0;

    bool m_need_recalculation = true;
};
} // namespace ars_graphics
