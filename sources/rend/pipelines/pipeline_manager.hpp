#pragma once
#include <cstdint>
#include <map>
#include <string>

#include "dynamic_vieport_pipeline_creater.hpp"
#include "pipeline.hpp"
#include "pipeline_creater.hpp"
#include "pipeline_layout_storage.hpp"

namespace ars_graphics
{

enum class PipelineStorageType
{
    STANDART_MODEL
};

using PipelineStorage = std::unordered_map<uint64_t, Pipeline>;

class PipelineManager
{
public:
    PipelineManager(const LogicalDevice& device,
                    const ShaderManager& shader_manager,
                    const DescriptorManager& descriptor_manager,
                    const vk::Extent2D& extent);

    const vk::PipelineLayout& getLayout(PipelineLayoutType type) const;

    const Pipeline* getPipeline(
        uint64_t vertex_int_representation,
        PipelineStorageType pipeline_storage_type,
        const vk::RenderPass& render_pass,
        const MainPipelineConfigInfo& main_pipeline_config_info)
    {
        PipelineStorage& storage = m_storages.at(pipeline_storage_type);

        auto&& it = storage.find(vertex_int_representation);

        if (it == storage.end())
        {
            PipelineConfigInfo config_info = {.main_config_info =
                                                  main_pipeline_config_info,
                                              .extent = m_extent};
            Pipeline pipeline{m_creater, render_pass, config_info};

            auto&& result =
                storage.emplace(vertex_int_representation, std::move(pipeline));
            return &(result.first->second);
        }
        else
        {
            return &(it->second);
        }
    }

private:
    DynamicViewportPipelineCreater m_creater;

    vk::Extent2D m_extent;

    PipelineLayoutStorage m_layout_storage;

    std::map<PipelineStorageType, PipelineStorage> m_storages;
};
} // namespace ars_graphics
