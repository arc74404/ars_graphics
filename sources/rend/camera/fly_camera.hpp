#pragma once

#include "interface_camera.hpp"

namespace ars_graphics
{

struct FlyCameraSettings
{
    glm::vec3 m_world_up = glm::vec3(0.f, 1.f, 0.f);

    // settings
    float m_view_shift_speed = 12;
    float m_movement_speed   = 0.5f;
    float m_near_plane       = 0.1f;
    float m_far_plane        = 50.0f;
    float m_up_down_speed    = 3;
};

class FlyCamera : public ICamera
{
public:
    FlyCamera(uint32_t width,
              uint32_t height,
              const FlyCameraSettings& setings);

private:
    void processKeyboardImpl(Key key,
                             KeyStatus status,
                             double delta_time) override;

    void processMouseMovementImpl(const glm::vec2& shift,
                                  double delta_time,
                                  bool constrain_pitch) override;

    void processMouseScrollImpl(float yoffset) override;

    glm::mat4 recalculateImpl() override;

    FlyCameraSettings m_settings;

    float m_zoom = 45.0f;

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_right;

    float m_yaw;
    float m_pitch;
    float m_aspect_ratio;
};
} // namespace ars_graphics
