#include "model_manager.hpp"

namespace ars_graphics
{
ModelManager::ModelManager(const LogicalDevice& logical_device,
                           const PhysicalDevice& physical_device,
                           const std::vector<std::string>& paths)
{
    TextureCreater texture_creater{logical_device, physical_device};

    m_loader.load(m_texture_data_storage, m_texture_storage, texture_creater,
                  paths, m_models);
}
} // namespace ars_graphics
