#pragma once

#include "../../gui/events/key.hpp"
#include "glm/glm.hpp"

namespace ars_graphics
{
class ICamera
{
public:
    virtual void processKeyboard(Key key,
                                 KeyStatus status,
                                 double delta_time)                = 0;
    virtual void processMouseMovement(const glm::vec2& shift,
                                      double delta_time,
                                      bool constrain_pitch = true) = 0;
    virtual void processMouseScroll(float yoffset)                 = 0;
    virtual glm::mat4 recalculate()                                = 0;

    bool needRecalculation() const
    {
        return m_need_recalculation;
    }

protected:
    bool m_need_recalculation = true;
};
} // namespace ars_graphics
