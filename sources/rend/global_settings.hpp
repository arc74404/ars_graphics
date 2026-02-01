#pragma once
#include <cstdint>

namespace ars_graphics::settings
{
namespace bindings
{
constexpr uint32_t vertex_attributes_binding = 0;

constexpr uint32_t uniform_buffer_binding = 9;

constexpr uint32_t instancing_binding = 15;

constexpr uint32_t material_shader_binding = 0;
} // namespace bindings

constexpr uint8_t bytes_per_pixel = 4;
} // namespace ars_graphics::settings
