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

    void init(const PipelineConfigInfo& pipeline_core_config_info);

    vk::UniquePipeline createPipeline(const ShaderManager& shader_manager,
                                      const vk::RenderPass& render_pass,
                                      const LogicalDevice& device);

private:
    // convert to vulkan config func

    vk::GraphicsPipelineCreateInfo convertToVulkanConfigInfo(
        const vk::RenderPass& render_pass);

    // ---- //

    void createShaderStages(const ShaderManager& shader_manager);

    virtual vk::PipelineVertexInputStateCreateInfo vertexInputState();
    virtual vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState();
    virtual vk::PipelineRasterizationStateCreateInfo rasterizerState();
    virtual vk::PipelineMultisampleStateCreateInfo multisamplingState();
    virtual vk::PipelineColorBlendStateCreateInfo colorBlendState();
    virtual vk::PipelineDepthStencilStateCreateInfo depthStencil();
    virtual vk::PipelineViewportStateCreateInfo viewportState();
    virtual vk::PipelineShaderStageCreateInfo vertexShaderinfo(
        vk::ShaderStageFlagBits flag,
        const vk::ShaderModule& shader_module);

private:
    PipelineConfigInfo m_config_info;

    vk::Viewport m_viewport;
    vk::Rect2D m_scissor;
    vk::PipelineColorBlendAttachmentState m_color_blend_attachment;

    std::vector<vk::PipelineShaderStageCreateInfo> m_shader_stages;

    vk::PipelineVertexInputStateCreateInfo m_vertex_input_state;

    vk::PipelineInputAssemblyStateCreateInfo m_input_assembly;

    vk::PipelineViewportStateCreateInfo m_viewport_state;

    vk::PipelineRasterizationStateCreateInfo m_rasterizer;

    vk::PipelineMultisampleStateCreateInfo m_multisampling;

    vk::PipelineColorBlendStateCreateInfo m_color_blending;

    vk::PipelineDepthStencilStateCreateInfo m_depth_stencil;

    vk::ShaderModule m_vertex_shader_module;
    vk::ShaderModule m_fragment_shader_module;
};
} // namespace ars_graphics
