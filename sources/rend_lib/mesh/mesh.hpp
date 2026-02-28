#pragma once
#include <vector>

#include "../material/material.hpp"
#include "../pipelines/pipeline.hpp"
#include "../pipelines/pipeline_layout_storage.hpp"
#include "../pipelines/pipeline_manager.hpp"
#include "../shaders/calculate_vertex_shader_type.hpp"
#include "../vertex/vertex.hpp"

#include "primitive.hpp"

namespace ars_graphics
{
class Mesh final
{
public:
    Mesh() = default;

    void addPrimitive(Primitive&& primitive);

    const std::vector<Primitive>& getPrimitives() const;

private:
    std::vector<Primitive> m_primitives;
};
} // namespace ars_graphics
