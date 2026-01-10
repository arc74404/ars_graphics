#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include <optional>
#include <utility>

#include "../device/logical_device.hpp"
#include "../shaders/shader_manager.hpp"

#include "pipeline_config_info.hpp"

namespace ars_graphics
{

class PipelineCreater
{
public:
    virtual ~PipelineCreater() = default;

    PipelineCreater(const LogicalDevice& device,
                    const ShaderManager& shader_manager);

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
        const vk::RenderPass& render_pass);

    // ---- //

    void createShaderStages(
        std::vector<vk::PipelineShaderStageCreateInfo>& shader_stages,
        const PipelineConfigInfo& config_info,
        vk::ShaderModule& vertex_shader_module,
        vk::ShaderModule& fragment_shader_module);

    virtual vk::PipelineVertexInputStateCreateInfo vertexInputState(
        const PipelineConfigInfo& config_info);
    virtual vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState(
        const PipelineConfigInfo& config_info);
    virtual vk::PipelineRasterizationStateCreateInfo rasterizerState(
        const PipelineConfigInfo& config_info);
    virtual vk::PipelineMultisampleStateCreateInfo multisamplingState(
        const PipelineConfigInfo& config_info);
    virtual vk::PipelineColorBlendStateCreateInfo colorBlendState(
        const PipelineConfigInfo& config_info,
        vk::PipelineColorBlendAttachmentState& color_blend_attachment);
    virtual vk::PipelineDepthStencilStateCreateInfo depthStencil(
        const PipelineConfigInfo& config_info);
    virtual vk::PipelineViewportStateCreateInfo viewportState(
        const PipelineConfigInfo& config_info,
        vk::Viewport& viewport,
        vk::Rect2D& scissors);
    virtual vk::PipelineShaderStageCreateInfo shaderinfo(
        vk::ShaderStageFlagBits flag,
        const vk::ShaderModule& shader_module);

private:
    const LogicalDevice& m_device;
    const ShaderManager& m_shader_manager;
};
} // namespace ars_graphics
