
#pragma once

#include "../vertex/all_attributes.hpp"

#include "mesh.hpp"

namespace ars_graphics::primitive_types
{
// ========== 3D MESHES ==========

// Minimal mesh (position only)
using PositionOnly3D = Primitive<attributes::Position3D>;

// Simple mesh (position + texture coordinates)
using Simple3D = Primitive<attributes::Position3D, attributes::TextureCoord>;

// Standard mesh for PBR (position + normal + texture coordinates)
using Standart3D = Primitive<attributes::Position3D,
                             attributes::Normal,
                             attributes::TextureCoord>;

// Colored mesh (no textures)
using Colored3D = Primitive<attributes::Position3D, attributes::ColorRGBA>;

// Particle mesh (for particle systems)
using ParticleMesh = Primitive<attributes::Position3D,
                               attributes::ColorRGBA,
                               attributes::TextureCoord>;

#define PrimitivesPriorityListPack                \
    ars_graphics::primitive_types::Standart3D,    \
        ars_graphics::primitive_types::Colored3D, \
        ars_graphics::primitive_types::Simple3D,  \
        ars_graphics::primitive_types::PositionOnly3D

} // namespace ars_graphics::primitive_types
