#pragma once

#include <fstream>
#include <iostream>

#include "../mesh/mesh_types.hpp"
#include "../vertex/all_attributes.hpp"

#include "load_funcs.hpp"
#include "model_manager.hpp"
#include "model_path.hpp"
#include "tiny_gltf.h"

namespace ars_graphics
{

template <typename ModelManagerType>
class ModelLoader
{
public:
    void load(const std::vector<std::string>& paths,
              ModelManagerType& model_manager)
    {
        for (auto&& path : paths)
        {
            if (false == fileOpenSuccess(path))
            {
                std::cout << "Failed open file: " << path << '\n';
                continue;
            }
            tinygltf::Model model;
            if (false == loadGLTFModel(path, isBinary(path), model))
            {
                std::cout << "Failed loadGLTFModel: " << path << '\n';
                continue;
            }

            if (false ==
                loadImpl<PriorityListPackSmall>(model, model_manager, path))
            {
                std::cout << "Failed load model: " << path << '\n';
            }
        }
    }

private:
    bool fileOpenSuccess(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (false == file.is_open())
        {
            return false;
        }
        file.close();
        return true;
    }

    template <typename... MeshTypes>
    bool loadImpl(tinygltf::Model& model,
                  ModelManagerType& model_manager,
                  const std::string& path)
    {
        return (loadStandartModel<MeshTypes>(model, model_manager, path) ||
                ...);
        // return true;
    }
};

} // namespace ars_graphics
