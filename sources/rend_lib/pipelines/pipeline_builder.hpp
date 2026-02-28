#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "../../libs_includes/vulkan.hpp"
#include "../device/logical_device.hpp"

#include "pipeline_config_info.hpp"

namespace ars_graphics
{

struct PipelineBindInfo;

class PipelineBuilder final
{
public:
    // virtual ~PipelineBuilder() = default;

    PipelineBuilder(vk::Device logical_device);

    vk::UniquePipeline createPipeline(const vk::RenderPass& render_pass,
                                      const PipelineConfigInfo& config_info);

private:
    // convert to vulkan config func

    vk::GraphicsPipelineCreateInfo convertToVulkanConfigInfo(
        const PipelineConfigInfo& config_info,
        const std::vector<vk::PipelineShaderStageCreateInfo>&,
        const vk::PipelineVertexInputStateCreateInfo&,
        const vk::PipelineInputAssemblyStateCreateInfo&,
        const vk::PipelineViewportStateCreateInfo&,
        const vk::PipelineRasterizationStateCreateInfo&,
        const vk::PipelineMultisampleStateCreateInfo&,
        const vk::PipelineColorBlendStateCreateInfo&,
        const vk::PipelineDepthStencilStateCreateInfo&,
        const vk::PipelineDynamicStateCreateInfo&,
        const vk::RenderPass& render_pass);

    // ---- //

    void createShaderStages(
        std::vector<vk::PipelineShaderStageCreateInfo>& shader_stages,
        const PipelineConfigInfo& config_info);

    vk::PipelineVertexInputStateCreateInfo vertexInputState(
        const PipelineConfigInfo& config_info);
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState(
        const PipelineConfigInfo& config_info);
    vk::PipelineRasterizationStateCreateInfo rasterizerState(
        const PipelineConfigInfo& config_info);
    vk::PipelineMultisampleStateCreateInfo multisamplingState(
        const PipelineConfigInfo& config_info);
    vk::PipelineColorBlendStateCreateInfo colorBlendState(
        const PipelineConfigInfo& config_info,
        vk::PipelineColorBlendAttachmentState& color_blend_attachment);
    vk::PipelineDepthStencilStateCreateInfo depthStencil(
        const PipelineConfigInfo& config_info);
    vk::PipelineViewportStateCreateInfo viewportState(
        const PipelineConfigInfo& config_info,
        vk::Viewport& viewport,
        vk::Rect2D& scissors);
    vk::PipelineShaderStageCreateInfo shaderinfo(
        vk::ShaderStageFlagBits flag,
        const vk::ShaderModule& shader_module);

    vk::PipelineDynamicStateCreateInfo dynamicStates(
        const PipelineConfigInfo& config_info);

protected:
    vk::Device m_device;
};
} // namespace ars_graphics
