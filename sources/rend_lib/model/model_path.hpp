#pragma once

#include <string>

namespace ars_graphics
{
template <typename MeshT>
struct ModelPath
{
    using MeshType = MeshT;

    std::string m_path;
};
} // namespace ars_graphics
