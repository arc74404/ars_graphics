#include "model_manager.hpp"

#include "../material/material_creater.hpp"

namespace ars_graphics
{
ModelManager::ModelManager(const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device,
                           const DescriptorManager& descriptor_manager,
                           const std::vector<std::string>& paths)
    : m_texture_storage(logical_device, physical_device),
      m_texture_creater(logical_device, physical_device),
      m_material_creater(logical_device, physical_device, descriptor_manager)
{
    for (auto&& p : paths)
    {
        load(p);
    }
}

void
ModelManager::load(const std::string& path)
{
    TextureDataStorage texture_data_storage;

    if (false == fileOpenSuccess(path))
    {
        std::cout << "Failed open file: " << path << '\n';
        return;
    }
    tinygltf::Model gltf_model;
    if (false == loadGLTFModel(path, isBinary(path), gltf_model))
    {
        std::cout << "Failed loadGLTFModel: " << path << '\n';
        return;
    }

    Model res_model;

    std::vector<TextureData> textures_data;

    if (false == loadTextures(gltf_model.textures, gltf_model, textures_data))
    {
        std::cout << "Failed load textures: " << path << '\n';
        return;
    }

    std::vector<TextureData>& realoc_textures_data =
        texture_data_storage.pushDataTextures(std::move(textures_data));

    std::vector<ars_graphics::MaterialData> materials_data;

    if (false == loadMaterials(gltf_model.materials, realoc_textures_data,
                               m_texture_creater, m_texture_storage,
                               materials_data))
    {
        std::cout << "Failed load materials: " << path << '\n';
        return;
    }

    std::vector<ars_graphics::Material> convert_data =
        m_material_creater.convertToMaterials(materials_data);

    std::vector<Material>& realoc_materials =
        m_material_storage.pushMaterials(std::move(convert_data));

    for (auto&& gltf_mesh : gltf_model.meshes)
    {
        bool checker = loadMesh<VerticesPriorityListPack>(
            gltf_model, gltf_mesh, res_model, realoc_materials);

        if (false == checker)
        {
            std::cout << "Failed load mesh\n";
            return;
        }
    }

    m_models.emplace(path, std::move(res_model));

    // if (false == loadImpl(pipeline_manager, render_pass, gltf_model,
    //                       model_storage, texture_data_storage,
    //                       texture_storage, texture_creater, material_storage,
    //                       material_creater, path))
    // {
    //     std::cout << "Failed load model: " << path << '\n';
    // }
}

const Model*
ModelManager::operator[](const std::string& key)
{
    auto&& it = m_models.find(key);
    if (it == m_models.end())
    {
        throw std::runtime_error(std::format("{}{}", "No such model: ", key));
    }
    return &(it->second);
}

} // namespace ars_graphics
