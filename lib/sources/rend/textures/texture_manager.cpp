#include "texture_manager.hpp"

#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace ars_graphics
{

void
TextureManager::loadTextures(const LogicalDevice& logical_device,
                             const PhysicalDevice& physical_device,
                             const std::vector<std::string>& paths,
                             const DescriptorManager& desc_manager)
{
    // Atlases
    for (auto& path : paths)
    {
        addTexture(logical_device, physical_device, path, desc_manager);
    }
}

void
TextureManager::addTexture(const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device,
                           const std::string& file_path,
                           const DescriptorManager& descriptor_manager)
{
    if (m_texture_storage.contains(file_path))
    {
        std::cerr << file_path << " has already been added\n";
        return;
    }

    Texture new_texture{logical_device, physical_device};

    new_texture.loadTexture(logical_device, physical_device, file_path);

    m_texture_storage.emplace(file_path, std::move(new_texture));
}

const Texture* const
TextureManager::getTexture(const std::string& name) const
{
    auto it = m_texture_storage.find(name);
    if (it == m_texture_storage.end())
    {
        throw "cant find in texture_zone_storage\n";
    }

    return &(it->second);
}
}; // namespace ars_graphics
