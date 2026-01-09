#pragma once
#include <map>

#include "pipeline.hpp"
#include "pipeline_creater.hpp"
#include "pipeline_layout_storage.hpp"

namespace ars_graphics
{

enum class PipelineStorageType
{
    STANDART_MODEL
};

using PipelineStorage = std::unordered_map<std::string, Pipeline>;

class PipelineManager
{
public:
    PipelineManager(const LogicalDevice& device,
                    const ShaderManager& shader_manager,
                    const DescriptorManager& descriptor_manager,
                    const vk::Extent2D& extent);

    const vk::PipelineLayout& getLayout(PipelineLayoutType type) const;

    template <typename VertexType>
    const Pipeline* getPipeline(
        PipelineStorageType pipeline_storage_type,
        const vk::RenderPass& render_pass,
        const MainPipelineConfigInfo& main_pipeline_config_info)
    {
        PipelineStorage& storage = m_storages.find(pipeline_storage_type);

        std::string key = VertexType::getStrRepersentation();

        auto&& it = storage.find(key);

        if (it == storage.end())
        {
            PipelineConfigInfo config_info = {.main_config_info =
                                                  main_pipeline_config_info,
                                              .extent = m_extent};
            storage.emplace(key,
                            m_creater.createPipeline(render_pass, config_info));
        }
        return storage.at(key);
    }

private:
    PipelineCreater m_creater;

    vk::Extent2D m_extent;

    PipelineLayoutStorage m_layout_storage;

    std::map<PipelineStorageType, PipelineStorage> m_storages;
};
} // namespace ars_graphics
