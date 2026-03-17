#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../material/material.hpp"
#include "../textures/texture_creater.hpp"
#include "../textures/texture_data.hpp"
#include "../textures/texture_data_storage.hpp"
#include "../textures/texture_storage.hpp"
#include "../vertex/all_attributes.hpp"
#include "../vertex/vertex_types.hpp"

#include "load_funcs.hpp"
#include "model.hpp"
#include "model_path.hpp"
#include "tiny_gltf.h"

namespace ars_graphics
{
class ModelManager
// {
// public:
//     ModelManager(const LogicalDevice& logical_device,
//                  const PhysicalDevice& physical_device,
//                  const DescriptorManager& descriptor_manager,
//                  const std::vector<std::string>& paths);

//     const Model* operator[](const std::string& key);

// private:
//     bool fileOpenSuccess(const std::string& path) const;

//     void load(const std::string& path);

//     bool loadTextures(const std::vector<tinygltf::Texture>& gltf_textures,
//                       const tinygltf::Model& gltf_model,
//                       std::vector<TextureData>& textures_data);

//     std::optional<TextureData> loadTexture(const tinygltf::Image& image,
//                                            const tinygltf::Model&
//                                            gltf_model);

//     bool loadMaterials(const std::vector<tinygltf::Material>& gltf_materials,
//                        const std::vector<TextureData>& textures_data,
//                        const TextureCreater& texture_creater,
//                        TextureStorage& texturue_storage,
//                        std::vector<MaterialData>& materials_data);

//     TextureCreater m_texture_creater;

//     TextureStorage m_texture_storage;

//     std::unordered_map<std::string, Model> m_models;
// };
// 

} // namespace ars_graphics
