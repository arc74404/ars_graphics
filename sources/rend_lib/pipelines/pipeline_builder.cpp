#include "pipeline_builder.hpp"

#include "../util_funcs.hpp"

#include "pipeline.hpp"

namespace ars_graphics
{

PipelineBuilder::PipelineBuilder(vk::Device logical_device)
    : m_device(logical_device)
{
}

vk::GraphicsPipelineCreateInfo
PipelineBuilder::convertToVulkanConfigInfo(
    const PipelineConfigInfo& config_info,
    const std::vector<vk::PipelineShaderStageCreateInfo>& shader_stages,
    const vk::PipelineVertexInputStateCreateInfo& vertex_input_state,
    const vk::PipelineInputAssemblyStateCreateInfo& input_assembly,
    const vk::PipelineViewportStateCreateInfo& viewport_state,
    const vk::PipelineRasterizationStateCreateInfo& rasterizer,
    const vk::PipelineMultisampleStateCreateInfo& multisampling,
    const vk::PipelineColorBlendStateCreateInfo& color_blending,
    const vk::PipelineDepthStencilStateCreateInfo& depth_stencil,
    const vk::PipelineDynamicStateCreateInfo& dynamic_states,
    const vk::RenderPass& render_pass)
{
    vk::GraphicsPipelineCreateInfo create_pipeline_info{};

    // must be init:
    create_pipeline_info.stageCount          = shader_stages.size();
    create_pipeline_info.pStages             = shader_stages.data();
    create_pipeline_info.pVertexInputState   = &vertex_input_state;
    create_pipeline_info.pInputAssemblyState = &input_assembly;
    create_pipeline_info.pViewportState      = &viewport_state;
    create_pipeline_info.pRasterizationState = &rasterizer;
    create_pipeline_info.pMultisampleState   = &multisampling;
    create_pipeline_info.pColorBlendState    = &color_blending;
    create_pipeline_info.pDynamicState       = &dynamic_states;
    create_pipeline_info.layout              = config_info.pipeline_layout;
    create_pipeline_info.renderPass          = render_pass;
    create_pipeline_info.subpass             = 0;

    // optional:
    create_pipeline_info.flags = vk::PipelineCreateFlags();
    create_pipeline_info.pTessellationState;
    create_pipeline_info.pDynamicState;
    create_pipeline_info.pDepthStencilState = &depth_stencil;
    create_pipeline_info.basePipelineHandle;
    create_pipeline_info.basePipelineIndex = -1;

    return create_pipeline_info;
}

void
PipelineBuilder::createShaderStages(
    std::vector<vk::PipelineShaderStageCreateInfo>& shader_stages,
    const PipelineConfigInfo& config_info)
{
    shader_stages.push_back(shaderinfo(vk::ShaderStageFlagBits::eVertex,
                                       config_info.vertex_shader_module));

    shader_stages.push_back(shaderinfo(vk::ShaderStageFlagBits::eFragment,
                                       config_info.fragment_shader_module));
}

vk::UniquePipeline
PipelineBuilder::createPipeline(const vk::RenderPass& render_pass,
                                const PipelineConfigInfo& config_info)
{
    vk::Viewport viewport;
    vk::Rect2D scissors;

    vk::PipelineColorBlendAttachmentState color_blend_attachment;

    std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;
    createShaderStages(shader_stages, config_info);

    //////////////////

    vk::PipelineVertexInputStateCreateInfo m_vertex_input_state =
        vertexInputState(config_info);
    vk::PipelineInputAssemblyStateCreateInfo m_input_assembly =
        inputAssemblyState(config_info);
    vk::PipelineViewportStateCreateInfo m_viewport_state =
        viewportState(config_info, viewport, scissors);
    vk::PipelineRasterizationStateCreateInfo m_rasterizer =
        rasterizerState(config_info);
    vk::PipelineMultisampleStateCreateInfo m_multisampling =
        multisamplingState(config_info);
    vk::PipelineColorBlendStateCreateInfo m_color_blending =
        colorBlendState(config_info, color_blend_attachment);
    vk::PipelineDepthStencilStateCreateInfo m_depth_stencil =
        depthStencil(config_info);
    vk::PipelineDynamicStateCreateInfo m_dynamic_states =
        dynamicStates(config_info);

    auto&& res = m_device.createGraphicsPipelineUnique(
        nullptr,
        convertToVulkanConfigInfo(
            config_info, shader_stages, m_vertex_input_state, m_input_assembly,
            m_viewport_state, m_rasterizer, m_multisampling, m_color_blending,
            m_depth_stencil, m_dynamic_states, render_pass));

    if (res.result != vk::Result::eSuccess)
    {
        throw std::logic_error("Failed create pipeline");
    }
    return {std::move(res.value)};
}

vk::PipelineInputAssemblyStateCreateInfo
PipelineBuilder::inputAssemblyState(const PipelineConfigInfo& config_info)
{
    vk::PipelineInputAssemblyStateCreateInfo input_assembly_info;
    input_assembly_info.flags    = vk::PipelineInputAssemblyStateCreateFlags();
    input_assembly_info.topology = vk::PrimitiveTopology::eTriangleList;
    return input_assembly_info;
}

vk::PipelineViewportStateCreateInfo
PipelineBuilder::viewportState(const PipelineConfigInfo& config_info,
                               vk::Viewport& viewport,
                               vk::Rect2D& scissors)
{
    viewport.x        = 0.0f;
    viewport.y        = 0.0f;
    viewport.width    = config_info.extent.width;
    viewport.height   = config_info.extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    scissors.offset.x = 0.0f;
    scissors.offset.y = 0.0f;
    scissors.extent   = config_info.extent;

    vk::PipelineViewportStateCreateInfo viewport_state = {};
    viewport_state.flags         = vk::PipelineViewportStateCreateFlags();
    viewport_state.viewportCount = 1;
    viewport_state.pViewports    = &viewport;
    viewport_state.scissorCount  = 1;
    viewport_state.pScissors     = &scissors;

    if (config_info.dynamic_states.size() > 0)
    {
        viewport_state.pViewports = nullptr;
        viewport_state.pScissors  = nullptr;
    }

    return viewport_state;
}

vk::PipelineDynamicStateCreateInfo
PipelineBuilder::dynamicStates(const PipelineConfigInfo& config_info)
{
    vk::PipelineDynamicStateCreateInfo res;

    res.dynamicStateCount = config_info.dynamic_states.size();
    res.pDynamicStates    = config_info.dynamic_states.data();

    return res;
}

vk::PipelineRasterizationStateCreateInfo
PipelineBuilder::rasterizerState(const PipelineConfigInfo& config_info)
{
    vk::PipelineRasterizationStateCreateInfo rasterizer;

    rasterizer.flags            = vk::PipelineRasterizationStateCreateFlags();
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode             = vk::PolygonMode::eFill;
    rasterizer.lineWidth               = 1.0f;
    rasterizer.cullMode                = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace               = vk::FrontFace::eCounterClockwise;
    rasterizer.depthBiasEnable         = VK_FALSE;

    return rasterizer;
}

vk::PipelineMultisampleStateCreateInfo
PipelineBuilder::multisamplingState(const PipelineConfigInfo& config_info)
{
    vk::PipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
    multisampling.sampleShadingEnable  = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
    return multisampling;
}

vk::PipelineColorBlendStateCreateInfo
PipelineBuilder::colorBlendState(
    const PipelineConfigInfo& config_info,
    vk::PipelineColorBlendAttachmentState& color_blend_attachment)
{
    color_blend_attachment.blendEnable = vk::True;
    color_blend_attachment.colorWriteMask =
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

    color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;

    color_blend_attachment.dstColorBlendFactor =
        vk::BlendFactor::eOneMinusSrcAlpha;

    color_blend_attachment.colorBlendOp        = vk::BlendOp::eAdd;
    color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
    color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    color_blend_attachment.alphaBlendOp        = vk::BlendOp::eAdd;

    vk::PipelineColorBlendStateCreateInfo color_blending = {};
    color_blending.flags             = vk::PipelineColorBlendStateCreateFlags();
    color_blending.logicOpEnable     = vk::False;
    color_blending.logicOp           = vk::LogicOp::eCopy;
    color_blending.attachmentCount   = 1;
    color_blending.pAttachments      = &color_blend_attachment;
    color_blending.blendConstants[0] = 0.0f;
    color_blending.blendConstants[1] = 0.0f;
    color_blending.blendConstants[2] = 0.0f;
    color_blending.blendConstants[3] = 0.0f;

    return color_blending;
}

vk::PipelineShaderStageCreateInfo
PipelineBuilder::shaderinfo(vk::ShaderStageFlagBits flag,
                            const vk::ShaderModule& shader_module)
{
    vk::PipelineShaderStageCreateInfo vertex_shader_info = {};
    vertex_shader_info.flags  = vk::PipelineShaderStageCreateFlags();
    vertex_shader_info.stage  = flag;
    vertex_shader_info.module = shader_module;
    vertex_shader_info.pName  = "main";
    return vertex_shader_info;
}

vk::PipelineVertexInputStateCreateInfo
PipelineBuilder::vertexInputState(const PipelineConfigInfo& config_info)
{
    vk::PipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.flags = vk::PipelineVertexInputStateCreateFlags();

    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.pVertexBindingDescriptions =
        &config_info.vertex_binding_description;

    vertex_input_info.vertexAttributeDescriptionCount =
        config_info.vertex_attribute_descriptions.size();
    vertex_input_info.pVertexAttributeDescriptions =
        config_info.vertex_attribute_descriptions.data();

    return vertex_input_info;
}

vk::PipelineDepthStencilStateCreateInfo
PipelineBuilder::depthStencil(const PipelineConfigInfo& config_info)
{
    vk::PipelineDepthStencilStateCreateInfo depth_stencil_info{};

    depth_stencil_info.depthTestEnable   = config_info.depth_test_enable;
    depth_stencil_info.depthWriteEnable  = vk::True;
    depth_stencil_info.depthCompareOp    = vk::CompareOp::eLess;
    depth_stencil_info.stencilTestEnable = vk::False;

    return depth_stencil_info;
}
} // namespace ars_graphics
