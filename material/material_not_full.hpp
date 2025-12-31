#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <string>

namespace ars_graphics
{
class Material
{
public:
    virtual ~Material() = default;

    template <typename Vertex_t>
    void upgradeVerts(std::vector<Vertex_t>& verts) const;

    enum class Type
    {
        TEXTURE_ZONE
    };
    Material(Type t);

    virtual void bind(const vk::CommandBuffer& cmd,
                      const vk::PipelineLayout& layout) const = 0;

    Type getType() const noexcept;

private:
    template <typename Vertex_t, typename Material_t>
    void upgradeVerts_(std::vector<Vertex_t>& verts) const;

    Type m_type;
};

class TextureZoneMaterial;

template <typename Vertex_t>
void
Material::upgradeVerts(std::vector<Vertex_t>& verts) const
{
    switch (m_type)
    {
        case Type::TEXTURE_ZONE:
            upgradeVerts_<Vertex_t, TextureZoneMaterial>(verts);

            break;

        default:
            break;
    }
}

} // namespace ars_graphics
