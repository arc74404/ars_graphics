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
    ModelManager(PipelineManager& pipeline_manager,
                 const vk::RenderPass& render_pass,
                 const LogicalDevice& logical_device,
                 const PhysicalDevice& physical_device,
                 const DescriptorManager& descriptor_manager,
                 const vk::PipelineLayout& pipelayout,
                 const std::vector<std::string>& paths);

    const Model* operator[](const std::string& key);

private:
    TextureStorage m_texture_storage;

    MaterialStorage m_material_storage;

    ModelLoader m_loader;

    std::unordered_map<std::string, Model> m_models;
};

} // namespace ars_graphics
