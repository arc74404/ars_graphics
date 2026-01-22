#pragma once

#include "interface_camera.hpp"

namespace ars_graphics
{
class FlyCamera : public ICamera
{
public:
    FlyCamera(uint32_t width, uint32_t height);

    void processKeyboard(Key key, KeyStatus status, double delta_time) override;
    void processMouseMovement(const glm::vec2& shift,
                              double delta_time,
                              bool constrain_pitch = true) override;
    void processMouseScroll(float yoffset) override;

    glm::mat4 recalculate() override;

private:
    static glm::vec3 m_world_up;

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;

    float m_yaw;
    float m_pitch;

    // settings
    float m_view_shift_speed = 12;
    float m_movement_speed   = 0.5f;
    float m_zoom             = 45.0f;
    float m_near_plane       = 0.1f;
    float m_far_plane        = 50.0f;

    float m_aspect_ratio;
};
} // namespace ars_graphics
