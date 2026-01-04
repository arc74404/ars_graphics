#pragma once

#include <fstream>
#include <iostream>

#include "../mesh/primitives_types.hpp"
#include "../vertex/all_attributes.hpp"

#include "load_funcs.hpp"
#include "model_manager.hpp"
#include "model_path.hpp"

namespace ars_graphics
{

class ModelLoader
{
public:
    void load(const std::vector<std::string>& paths,
              ModelManager& model_manager) const;

private:
    bool fileOpenSuccess(const std::string& path) const;

    bool loadImpl(tinygltf::Model& gltf_model,
                  ModelManager& model_manager,
                  const std::string& path) const;

    bool loadTextures(const std::vector<tinygltf::Texture>& gltf_textures,
                      const tinygltf::Model& gltf_model) const;

    bool loadTexture(const tinygltf::Image& image,
                     const tinygltf::Model& gltf_model) const;
};

} // namespace ars_graphics
