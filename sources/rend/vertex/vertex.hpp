#pragma once

#include <tuple>

#include "all_attributes.hpp"

namespace ars_graphics
{

template <typename... Attributes>
class Vertex : public Attributes...
{
public:
    using TupleOfVectorAttributes = std::tuple<std::vector<Attributes>...>;

    using ContType =
        typename std::tuple_element<0,
                                    std::tuple<Attributes...>>::type::ContType;
    void pushBytes(ContType& cont)
    {
        (Attributes::pushBytes(cont), ...);
    }
};

using Default2DVertex = Vertex<attributes::Position2D,
                               attributes::OnlyAChannel,
                               attributes::TextureCoord>;

using Default3DVertex = Vertex<attributes::Position3D,
                               attributes::OnlyAChannel,
                               attributes::TextureCoord>;

}; // namespace ars_graphics
