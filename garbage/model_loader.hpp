#pragma once

#include <fstream>
#include <iostream>
#include <optional>

#include "../material/material.hpp"
#include "../material/material_creater.hpp"
#include "../material/material_storage.hpp"
#include "../textures/texture_creater.hpp"
#include "../textures/texture_data.hpp"
#include "../textures/texture_data_storage.hpp"
#include "../textures/texture_storage.hpp"
#include "../vertex/all_attributes.hpp"
#include "../vertex/vertex_types.hpp"

#include "load_funcs.hpp"
#include "model_path.hpp"
#include "tiny_gltf.h"

namespace ars_graphics
{

class ModelLoader
{
public:
    void load(PipelineManager& pipeline_manager,
              const vk::RenderPass& render_pass,
              TextureStorage& texture_storage,
              const TextureCreater& texture_creater,
              MaterialStorage& material_storage,
              const MaterialCreater& material_creater,
              const std::vector<std::string>& paths,
              std::unordered_map<std::string, Model>& model_storage) const;

private:
    bool fileOpenSuccess(const std::string& path) const;

    bool loadImpl(PipelineManager& pipeline_manager,
                  const vk::RenderPass& render_pass,
                  tinygltf::Model& gltf_model,
                  std::unordered_map<std::string, Model>& model_storage,
                  TextureDataStorage& texture_data_storage,
                  TextureStorage& texture_storage,
                  const TextureCreater& texture_creater,
                  MaterialStorage& material_storage,
                  const MaterialCreater& material_creater,
                  const std::string& path) const;

    bool loadTextures(const std::vector<tinygltf::Texture>& gltf_textures,
                      const tinygltf::Model& gltf_model,
                      std::vector<TextureData>& textures_data) const;

    std::optional<TextureData> loadTexture(
        const tinygltf::Image& image,
        const tinygltf::Model& gltf_model) const;

    bool loadMaterials(const std::vector<tinygltf::Material>& gltf_materials,
                       const std::vector<TextureData>& textures_data,
                       const TextureCreater& texture_creater,
                       TextureStorage& texturue_storage,
                       std::vector<MaterialData>& materials_data) const;
};

} // namespace ars_graphics
