#include "model_manager.hpp"

#include "../material/material_creater.hpp"

namespace ars_graphics
{
ModelManager::ModelManager(PipelineManager& pipeline_manager,
                           const vk::RenderPass& render_pass,
                           const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device,
                           const DescriptorManager& descriptor_manager,
                           const vk::PipelineLayout& pipelayout,
                           const std::vector<std::string>& paths)
    : m_texture_storage(logical_device, physical_device)
{
    std::cout << "texture creater\n";
    TextureCreater texture_creater{logical_device, physical_device};

    std::cout << "model creater\n";

    MaterialCreater material_creater(logical_device, physical_device,
                                     descriptor_manager, pipelayout);

    std::cout << "load\n";

    load(paths);
}

void
ModelManager::load(const std::vector<std::string>& paths) const
{
    TextureDataStorage texture_data_storage;

    for (auto&& path : paths)
    {
        if (false == fileOpenSuccess(path))
        {
            std::cout << "Failed open file: " << path << '\n';
            continue;
        }
        tinygltf::Model gltf_model;
        if (false == loadGLTFModel(path, isBinary(path), gltf_model))
        {
            std::cout << "Failed loadGLTFModel: " << path << '\n';
            continue;
        }

        if (false == loadImpl(pipeline_manager, render_pass, gltf_model,
                              model_storage, texture_data_storage,
                              texture_storage, texture_creater,
                              material_storage, material_creater, path))
        {
            std::cout << "Failed load model: " << path << '\n';
        }
    }
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
