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

    Pipeline(PipelineCreater& creater,
             const vk::RenderPass& render_pass,
             const PipelineConfigInfo& pipeline_config_info);

    void bind(const vk::CommandBuffer& cmd) const;

private:
    vk::UniquePipeline m_pipeline;
};
} // namespace ars_graphics
