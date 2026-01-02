
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
using Standard3D =
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

} // namespace ars_graphics::mesh_types
