#pragma once

#include <array>
#include <memory>

#include "glm/vec2.hpp"

#include "key.hpp"

namespace user_part
{
struct Event
{
    virtual ~Event() noexcept = default;
    enum class Type
    {
        CLOSE_WINDOW,
        MOVE_MOUSE,
        KEY
    };
    Type m_type;
    Event(Type t) : m_type(t)
    {
    }
};

using EventPtr = std::unique_ptr<Event>;

struct MoveMouseEvent final : Event
{
    MoveMouseEvent(const glm::vec2 shift)
        : Event(Type::MOVE_MOUSE), m_shift(shift)
    {
    }

    glm::vec2 m_shift;
};

struct CloseWindowEvent final : Event
{
    CloseWindowEvent() : Event(Type::CLOSE_WINDOW)
    {
    }
};

struct KeyEvent final : Event
{
    KeyEvent(Key key, KeyStatus status)
        : Event(Type::KEY), m_key(key), m_status(status)
    {
    }
    Key m_key;

    KeyStatus m_status;
};

} // namespace ars_graphics
