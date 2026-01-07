#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "../device/logical_device.hpp"
#include "../shaders/shader_manager.hpp"

#include "pipeline_config_info.hpp"

namespace ars_graphics
{
class DescriptorManager;
class PipelineCreater;

class Pipeline
{
public:
    virtual ~Pipeline() = default;

    Pipeline(std::unique_ptr<PipelineCreater>&& creater,
             const PipelineConfigInfo& pipeline_core_config_info,
             const vk::RenderPass& render_pass,
             const ShaderManager& shader_manager,
             const LogicalDevice& device);

    void bind(const vk::CommandBuffer& cmd) const;

private:
    vk::UniquePipeline m_pipeline;
};
} // namespace ars_graphics
