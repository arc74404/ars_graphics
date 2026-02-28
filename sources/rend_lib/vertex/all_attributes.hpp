#pragma once

#include <vector>

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

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

#define GET_STRIDE             \
    static uint32_t getSize()  \
    {                          \
        return sizeof(m_data); \
    }

#define GET_FORMAT(format)        \
    static vk::Format getFormat() \
    {                             \
        return format;            \
    }

#define SHIFT_NUM(index)                \
    static void shiftNum(uint64_t& num) \
    {                                   \
        num |= 1 << index;              \
    }

template <typename Derived, typename Cont = std::vector<char>>
struct AttributeBase
{
    using ContType = Cont;
    void pushBytes(Cont& cont) const
    {
        cont.resize(cont.size() + Derived::getSize());
        void* begin =
            static_cast<void*>(&cont[cont.size() - Derived::getSize()]);
        memcpy(begin, reinterpret_cast<const void*>(this), Derived::getSize());
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
    GET_STRIDE
    GET_FORMAT(vk::Format::eR32G32Sfloat)
    SHIFT_NUM(0)

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
    GET_STRIDE
    GET_FORMAT(vk::Format::eR32G32B32Sfloat)
    SHIFT_NUM(1)

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
    DATA_IMPL
    GET_STRIDE
    GET_FORMAT(vk::Format::eR32Sfloat)
    SHIFT_NUM(2)

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
    GET_STRIDE
    GET_FORMAT(vk::Format::eR32G32B32Sfloat)
    SHIFT_NUM(3)
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
    GET_STRIDE
    GET_FORMAT(vk::Format::eR32G32Sfloat)
    SHIFT_NUM(4)
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
    GET_STRIDE
    GET_FORMAT(vk::Format::eR32G32B32Sfloat)
    SHIFT_NUM(5)
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
    GET_STRIDE
    GET_FORMAT(vk::Format::eR32G32B32A32Sfloat)
    SHIFT_NUM(6)
protected:
    glm::vec4 m_data;
};

struct Tangent : AttributeBase<Tangent>
{
public:
    constexpr void setTangent(float x, float y, float z)
    {
        m_data.x = x;
        m_data.y = y;
        m_data.z = z;
    }
    constexpr void setTangent(const glm::vec3& tangent)
    {
        m_data = tangent;
    }

    DATA_IMPL
    GET_STRIDE
    GET_FORMAT(vk::Format::eR32G32B32Sfloat)
    SHIFT_NUM(7)
protected:
    glm::vec3 m_data;
};

// struct Bitangent : AttributeBase<Bitangent>
// {
// public:
//     constexpr void setBitangent(float x, float y, float z)
//     {
//         m_data.x = x;
//         m_data.y = y;
//         m_data.z = z;
//     }
//     constexpr void setBitangent(const glm::vec3& bitangent)
//     {
//         m_data = bitangent;
//     }

//     DATA_IMPL
//     GET_STRIDE
//     GET_FORMAT(vk::Format::eR32G32B32Sfloat)
//     SHIFT_NUM(8)
// protected:
//     glm::vec3 m_data;
// };

// struct BoneWeights : AttributeBase<BoneWeights>
// {
// public:
//     constexpr void setBoneWeights(float w0, float w1, float w2, float w3)
//     {
//         m_data.x = w0;
//         m_data.y = w1;
//         m_data.z = w2;
//         m_data.w = w3;
//     }
//     constexpr void setBoneWeights(const glm::vec4& weights)
//     {
//         m_data = weights;
//     }

//     DATA_IMPL
//     GET_STRIDE
//     GET_FORMAT(vk::Format::eR32G32B32A32Sfloat)
//     SHIFT_NUM(9)
// protected:
//     glm::vec4 m_data;
// };

// struct BoneIndices : AttributeBase<BoneIndices>
// {
// public:
//     constexpr void setBoneIndices(uint32_t i0,
//                                   uint32_t i1,
//                                   uint32_t i2,
//                                   uint32_t i3)
//     {
//         m_data.x = static_cast<float>(i0);
//         m_data.y = static_cast<float>(i1);
//         m_data.z = static_cast<float>(i2);
//         m_data.w = static_cast<float>(i3);
//     }
//     constexpr void setBoneIndices(const glm::uvec4& indices)
//     {
//         m_data = glm::vec4(indices);
//     }

//     DATA_IMPL
//     GET_STRIDE
//     GET_FORMAT(vk::Format::eR32G32B32A32Sfloat)
//     SHIFT_NUM(10)
// protected:
//     glm::vec4 m_data;
// };

} // namespace ars_graphics::attributes
