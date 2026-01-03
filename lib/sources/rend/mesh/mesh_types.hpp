
#pragma once

#include "../vertex/all_attributes.hpp"

#include "mesh.hpp"

namespace ars_graphics::mesh_types
{
// ========== 3D MESHES ==========

// Minimal mesh (position only)
using PositionOnly3D = Mesh<attributes::Position3D>;

// Simple mesh (position + texture coordinates)
using Simple3D = Mesh<attributes::Position3D, attributes::TextureCoord>;

// Standard mesh for PBR (position + normal + texture coordinates)
using Standart3D =
    Mesh<attributes::Position3D, attributes::Normal, attributes::TextureCoord>;

// Colored mesh (no textures)
using Colored3D = Mesh<attributes::Position3D, attributes::ColorRGBA>;

// Particle mesh (for particle systems)
using ParticleMesh = Mesh<attributes::Position3D,
                          attributes::ColorRGBA,
                          attributes::TextureCoord>;

// UI/2.5D mesh
using UIMesh = Mesh<attributes::Position3D,
                    attributes::TextureCoord,
                    attributes::ColorRGBA>;

// ========== 2D MESHES ==========

using PositionOnly2D = Mesh<attributes::Position2D>;

// Simple 2D mesh (for sprites)
using Simple2D = Mesh<attributes::Position2D, attributes::TextureCoord>;

// 2D mesh with color (for UI)
using Colored2D = Mesh<attributes::Position2D, attributes::ColorRGBA>;

// Text/font mesh
using TextMesh = Mesh<attributes::Position2D,
                      attributes::TextureCoord,
                      attributes::ColorRGBA>;

// ========== SPECIALIZED MESHES ==========

// Voxel mesh (Minecraft-style)
using VoxelMesh =
    Mesh<attributes::Position3D, attributes::TextureCoord, attributes::Normal>;

// Foliage mesh (billboard grass/leaves)
using FoliageMesh = Mesh<attributes::Position3D,
                         attributes::TextureCoord,
                         attributes::Normal,
                         attributes::ColorRGB>;

// Water mesh (with animation support)
using WaterMesh =
    Mesh<attributes::Position3D, attributes::TextureCoord, attributes::Normal>;

// Skybox/cubemap mesh
using SkyboxMesh = Mesh<attributes::Position3D, attributes::TextureCoord>;

// ========== COMBINED MESHES ==========

// Lightweight mesh (for LOD levels)
using LightweightMesh = Mesh<attributes::Position3D, attributes::TextureCoord>;

#define PriorityListPack                           \
    ars_graphics::mesh_types::FoliageMesh,         \
        ars_graphics::mesh_types::Standart3D,      \
        ars_graphics::mesh_types::VoxelMesh,       \
        ars_graphics::mesh_types::WaterMesh,       \
        ars_graphics::mesh_types::TextMesh,        \
        ars_graphics::mesh_types::Colored3D,       \
        ars_graphics::mesh_types::Simple3D,        \
        ars_graphics::mesh_types::LightweightMesh, \
        ars_graphics::mesh_types::SkyboxMesh,      \
        ars_graphics::mesh_types::Simple2D,        \
        ars_graphics::mesh_types::Colored2D,       \
        ars_graphics::mesh_types::PositionOnly3D,  \
        ars_graphics::mesh_types::PositionOnly2D

#define PriorityListPackSmall             \
    ars_graphics::mesh_types::Standart3D, \
        ars_graphics::mesh_types::PositionOnly3D

} // namespace ars_graphics::mesh_types
