#pragma once

#include <fstream>
#include <iostream>

#include "../mesh/mesh_types.hpp"
#include "../vertex/all_attributes.hpp"

#include "load_funcs.hpp"
#include "model_manager.hpp"
#include "model_path.hpp"

namespace ars_graphics
{

using ModelManagerConc = ModelManager<mesh_types::Standard3D>;

class ModelLoader
{
public:
    bool load(
        const std::vector<ModelPath<mesh_types::Standard3D>>& standart_3d_paths,
        ModelManagerConc& mesh_manager);

private:
    template <typename ModelT>
    bool loadImpl(const std::vector<ModelPath<ModelT>>& paths,
                  ModelManagerConc& model_manager);
};

template <>
inline bool
ModelLoader::loadImpl(
    const std::vector<ModelPath<mesh_types::Standard3D>>& paths,
    ModelManagerConc& model_manager)
{
    bool success = true;
    for (const auto& path : paths)
    {
        const std::string& filepath = path.m_path;

        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << filepath << std::endl;
            success = false;
            continue;
        }
        file.close();

        bool is_glb = filepath.find(".glb") != std::string::npos;

        try
        {
            auto model = loadStandardModel(filepath, is_glb);
            if (model)
            {
                model_manager.emplace<mesh_types::Standard3D>(
                    path.m_path, std::move(*model));
            }
            else
            {
                success = false;
            }
        }
        catch (std::runtime_error& error)
        {
            std::cerr << error.what() << '\n';
            success = false;
        }
    }
    return success;
}

} // namespace ars_graphics
