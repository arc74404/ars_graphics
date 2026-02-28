
#pragma once

#include "../vertex/all_attributes.hpp"
#include "../vertex/vertex.hpp"

namespace ars_graphics::vertex_types
{
// ========== 3D MESHES ==========

// Minimal mesh (position only)
using PositionOnly3D = Vertex<attributes::Position3D>;

// Simple mesh (position + texture coordinates)
using Simple3D = Vertex<attributes::Position3D, attributes::TextureCoord>;

// Standard mesh for PBR (position + normal + texture coordinates)
using Standart3D = Vertex<attributes::Position3D,
                          attributes::Normal,
                          attributes::TextureCoord>;

// Colored mesh (no textures)
using Colored3D = Vertex<attributes::Position3D, attributes::ColorRGBA>;

// Particle mesh (for particle systems)
using ParticleMesh = Vertex<attributes::Position3D,
                            attributes::ColorRGBA,
                            attributes::TextureCoord>;

#define VerticesPriorityListPack               \
    ars_graphics::vertex_types::Standart3D,    \
        ars_graphics::vertex_types::Colored3D, \
        ars_graphics::vertex_types::Simple3D,  \
        ars_graphics::vertex_types::PositionOnly3D

} // namespace ars_graphics::vertex_types
