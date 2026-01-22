#include "fly_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <iostream>

#include "glm/glm.hpp"

namespace ars_graphics
{

glm::vec3 FlyCamera::m_world_up = glm::vec3(0.f, 1.f, 0.f);

void
FlyCamera::processMouseScroll(float yoffset)
{
    m_zoom -= yoffset;

    m_zoom = std::clamp(m_zoom, 0.f, 89.f);
}

FlyCamera::FlyCamera(uint32_t width, uint32_t height)
    : m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_movement_speed(2.5f),
      m_zoom(45.0f),
      m_position{0.f, 0.f, 3.f},
      m_yaw{-90.f},
      m_pitch{0.f},
      m_aspect_ratio(static_cast<float>(width) / static_cast<float>(height))
{
}

void
FlyCamera::processKeyboard(Key key, KeyStatus status, double delta_time)
{
    m_need_recalculation = true;

    float velocity = m_movement_speed * delta_time;

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
            m_position.y -= velocity * 3;
            m_need_recalculation = true;
            break;
        case Key::LEFT_CONTROL:
            m_position.y += velocity * 3;
            m_need_recalculation = true;
            break;
        default:
            break;
    }
}

void
FlyCamera::processMouseMovement(const glm::vec2& shift,
                                double delta_time,
                                bool constrain_pitch)
{
    m_need_recalculation = true;
    m_yaw += shift.x * m_view_shift_speed * delta_time;
    m_pitch += shift.y * m_view_shift_speed * delta_time;

    if (constrain_pitch && m_pitch > 89.0f) m_pitch = 89.0f;
    if (constrain_pitch && m_pitch < -89.0f) m_pitch = -89.0f;
}

glm::mat4
FlyCamera::recalculate()
{
    m_need_recalculation = false;
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, m_world_up));
    m_up    = glm::normalize(glm::cross(m_right, m_front));

    return glm::perspective(glm::radians(m_zoom), m_aspect_ratio, m_near_plane,
                            m_far_plane) *
           glm::lookAt(m_position, m_position + m_front, m_up);
}
} // namespace ars_graphics
