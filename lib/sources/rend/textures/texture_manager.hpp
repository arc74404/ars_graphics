#pragma once
#include <optional>
#include <unordered_map>

#include "glm/vec2.hpp"
#include "nlohmann/json.hpp"

#include "texture.hpp"

namespace ars_graphics
{

class TextureManager
{
public:
    const Texture* const getTexture(const std::string& name) const;

    void loadTextures(const LogicalDevice& logical_device,
                      const PhysicalDevice& physical_device,
                      const std::vector<std::string>& paths,
                      const DescriptorManager& desc_manager);

private:
    void addTexture(const LogicalDevice& logical_device,
                    const PhysicalDevice& physical_device,
                    const std::string& file_path,
                    const DescriptorManager& desc_manager);

    std::unordered_map<std::string, Texture> m_texture_storage;
};

}; // namespace ars_graphics
