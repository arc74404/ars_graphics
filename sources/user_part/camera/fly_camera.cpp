#include "fly_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>

#include "glm/glm.hpp"

namespace ars_graphics
{

void
FlyCamera::processMouseScrollImpl(float yoffset)
{
    m_zoom -= yoffset;

    m_zoom = std::clamp(m_zoom, 0.f, 89.f);
}

FlyCamera::FlyCamera(uint32_t width,
                     uint32_t height,
                     const FlyCameraSettings& setings)
    : m_settings(setings),
      m_aspect_ratio(static_cast<float>(width) / static_cast<float>(height))
{
}

void
FlyCamera::processKeyboardImpl(Key key, KeyStatus status, double delta_time)
{
    m_need_recalculation = true;

    float velocity = m_settings.m_movement_speed * delta_time;

    switch (key)
    {
        case Key::D:
            m_position += velocity * m_right;
            m_need_recalculation = true;
            break;
        case Key::A:
            m_position -= velocity * m_right;
            m_need_recalculation = true;
            break;
        case Key::S:
            m_position -= velocity * m_front;
            m_need_recalculation = true;
            break;
        case Key::W:
            m_position += velocity * m_front;
            m_need_recalculation = true;
            break;
        case Key::SPACE:
            m_position.y += velocity * m_settings.m_up_down_speed;
            m_need_recalculation = true;
            break;
        case Key::LEFT_CONTROL:
            m_position.y -= velocity * m_settings.m_up_down_speed;
            m_need_recalculation = true;
            break;
        default:
            break;
    }
}

void
FlyCamera::processMouseMovementImpl(const glm::vec2& shift,
                                    double delta_time,
                                    bool constrain_pitch)
{
    m_need_recalculation = true;
    m_yaw += shift.x * m_settings.m_view_shift_speed * delta_time;
    m_pitch -= shift.y * m_settings.m_view_shift_speed * delta_time;

    if (constrain_pitch && m_pitch > 89.0f) m_pitch = 89.0f;
    if (constrain_pitch && m_pitch < -89.0f) m_pitch = -89.0f;
}

glm::mat4
FlyCamera::recalculateImpl()
{
    m_need_recalculation = false;
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right      = glm::normalize(glm::cross(m_front, m_settings.m_world_up));
    glm::vec3 up = glm::normalize(glm::cross(m_right, m_front));

    glm::mat4 projection =
        glm::perspective(glm::radians(m_zoom), m_aspect_ratio,
                         m_settings.m_near_plane, m_settings.m_far_plane);

    projection[1][1] *= -1;

    return projection * glm::lookAt(m_position, m_position + m_front, up);
}
} // namespace ars_graphics
