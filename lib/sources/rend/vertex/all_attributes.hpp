#pragma once

#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace ars_graphics::attributes
{

#define DATA_IMPL                                      \
    const char* dataImpl() const                       \
    {                                                  \
        return reinterpret_cast<const char*>(&m_data); \
    }

template <typename Derived, typename Cont = std::vector<char>>
struct AttributeBase
{
    using ContType = Cont;
    void PushBytes(Cont& cont)
    {
        cont.resize(cont.size() + sizeof(Derived));
        void* begin = static_cast<void*>(&cont[cont.size() - sizeof(Derived)]);
        memcpy(begin, reinterpret_cast<const void*>(this), sizeof(Derived));
    }
    const char* data() const
    {
        return static_cast<const Derived*>(this)->dataImpl();
    }
};

struct Position2D : AttributeBase<Position2D>
{
public:
    constexpr void setPosition2D(float x, float y)
    {
        m_data.x = x;
        m_data.y = y;
    }
    constexpr void setPosition2D(const glm::vec2& pos)
    {
        m_data = pos;
    }

    DATA_IMPL

protected:
    glm::vec2 m_data;
};

struct Position3D : AttributeBase<Position3D>
{
public:
    constexpr void setPosition3D(float x, float y, float z)
    {
        m_data.x = x;
        m_data.y = y;
        m_data.z = z;
    }
    constexpr void setPosition3D(const glm::vec3& pos)
    {
        m_data = pos;
    }

    DATA_IMPL

protected:
    glm::vec3 m_data;
};

struct OnlyAChannel : AttributeBase<OnlyAChannel>
{
public:
    constexpr void setA(float d)
    {
        m_data = d;
    }

protected:
    float m_data;
};

struct Normal : AttributeBase<Normal>
{
public:
    constexpr void setNormal(float x, float y, float z)
    {
        m_data.x = x;
        m_data.y = y;
        m_data.z = z;
    }
    constexpr void setNormal(const glm::vec3& normal)
    {
        m_data = normal;
    }

    DATA_IMPL

protected:
    glm::vec3 m_data;
};

struct TextureCoord : AttributeBase<TextureCoord>
{
public:
    constexpr void setCoord(float u, float v)
    {
        m_data.x = u;
        m_data.y = v;
    }
    constexpr void setCoord(const glm::vec2& coord)
    {
        m_data = coord;
    }

    DATA_IMPL

protected:
    glm::vec2 m_data;
};

struct ColorRGB : AttributeBase<ColorRGB>
{
public:
    constexpr void setColor(float r, float g, float b)
    {
        m_data.x = r;
        m_data.y = g;
        m_data.z = b;
    }
    constexpr void setColor(const glm::vec3& color)
    {
        m_data = color;
    }

    DATA_IMPL

protected:
    glm::vec3 m_data;
};

struct ColorRGBA : AttributeBase<ColorRGBA>
{
public:
    constexpr void setColor(float r, float g, float b, float a)
    {
        m_data.x = r;
        m_data.y = g;
        m_data.z = b;
        m_data.w = a;
    }
    constexpr void setColor(const glm::vec4& color)
    {
        m_data = color;
    }

    DATA_IMPL

protected:
    glm::vec4 m_data;
};

} // namespace ars_graphics::attributes
