#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../material/material_storage.hpp"
#include "../textures/texture_data_storage.hpp"
#include "../textures/texture_storage.hpp"

#include "model.hpp"
#include "model_loader.hpp"

namespace ars_graphics
{

class ModelManager
{
public:
    ModelManager(const LogicalDevice& logical_device,
                 const PhysicalDevice& physical_device,
                 const DescriptorManager& descriptor_manager,
                 const std::vector<std::string>& paths);

    const Model* operator[](const std::string& key);

private:
    bool fileOpenSuccess(const std::string& path) const;

    void load(const std::string& path);

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

    TextureCreater m_texture_creater;

    MaterialCreater m_material_creater;

    TextureStorage m_texture_storage;

    MaterialStorage m_material_storage;

    std::unordered_map<std::string, Model> m_models;
};

} // namespace ars_graphics
