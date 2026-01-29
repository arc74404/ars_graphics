#pragma once

#include <tuple>

#include "../global_settings.hpp"

#include "all_attributes.hpp"

namespace ars_graphics
{
template <typename... Attributes>
struct VertexBindingDescription
{
    VertexBindingDescription()
    {
        description.binding   = settings::bindings::vertex_attributes_binding;
        description.stride    = getStride();
        description.inputRate = vk::VertexInputRate::eVertex;
    }
    vk::VertexInputBindingDescription description;
};

template <typename... Attributes>
struct VertexAttributeDescription
{
    VertexAttributeDescription()
    {
        attributes.reserve(sizeof...(Attributes));

        uint32_t location = 0;
        uint32_t offset   = 0;
        (
            [&]
            {
                vk::VertexInputAttributeDescription attribute{};
                attribute.binding =
                    settings::bindings::vertex_attributes_binding;
                attribute.location = location++;
                attribute.format   = Attributes::getFormat();
                attribute.offset   = offset;

                attributes.push_back(attribute);

                offset += Attributes::getSize();
            }(),
            ...);
    }
    std::vector<vk::VertexInputAttributeDescription> attributes;
};

template <typename... Attributes>
class Vertex : public Attributes...
{
public:
    using TupleOfVectorAttributes = std::tuple<std::vector<Attributes>...>;

    using ContType =
        typename std::tuple_element<0,
                                    std::tuple<Attributes...>>::type::ContType;

    static std::string getStrRepersentation()
    {
        uint32_t num = 0;
        (Attributes::shiftNum(num), ...);

        return std::to_string(num);
    }

    static const vk::VertexInputBindingDescription&
    getVertexBindingDescription()
    {
        VertexBindingDescription<Attributes...> d;
        return d;
    }

    static const std::vector<vk::VertexInputAttributeDescription>&
    getVertexAttributeDescription()
    {
        static VertexAttributeDescription<Attributes...> d;
        return d.attributes;
    }

    static uint32_t getStride()
    {
        return (Attributes::getSize() + ...);
    }

    void pushBytes(ContType& cont) const
    {
        (Attributes::pushBytes(cont), ...);
    }
};

}; // namespace ars_graphics
