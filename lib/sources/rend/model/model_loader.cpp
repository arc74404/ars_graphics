#include "model_loader.hpp"

namespace ars_graphics
{
bool
ModelLoader::load(
    const std::vector<ModelPath<mesh_types::Standard3D>>& standart_3d_paths,
    ModelManager<mesh_types::Standard3D>& mesh_manager)
{
    return loadImpl(standart_3d_paths, mesh_manager);
}
} // namespace ars_graphics
