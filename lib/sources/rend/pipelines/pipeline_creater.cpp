#include "pipeline_creater.hpp"

#include "../util_funcs.hpp"

#include "pipeline.hpp"

namespace ars_graphics
{
void
PipelineCreater::init(const PipelineConfigInfo& pipeline_core_config_info)
{
    m_config_info = pipeline_core_config_info;

    using pcc = PipelineCreater;

    fillBoxesByPipeline(
        this, //----- vertex input state ----//
        std::make_pair(&m_vertex_input_state, &pcc::vertexInputState),
        //----- input assembly state ----//
        std::make_pair(&m_input_assembly, &pcc::inputAssemblyState),
        //---- viewport state -----//
        std::make_pair(&m_viewport_state, &pcc::viewportState),
        //----- rasterizer state ----//
        std::make_pair(&m_rasterizer, &pcc::rasterizerState),
        //----- multisampling state----//
        std::make_pair(&m_multisampling, &pcc::multisamplingState),
        //----- color blending state ----//
        std::make_pair(&m_color_blending, &pcc::colorBlendState),
        //----- depth stencil ----//
        std::make_pair(&m_depth_stencil, &pcc::depthStencil));
}

vk::GraphicsPipelineCreateInfo
PipelineCreater::convertToVulkanConfigInfo(const vk::RenderPass& render_pass)
{
    vk::GraphicsPipelineCreateInfo create_pipeline_info{};

    // must be init:
    create_pipeline_info.stageCount          = m_shader_stages.size();
    create_pipeline_info.pStages             = m_shader_stages.data();
    create_pipeline_info.pVertexInputState   = &m_vertex_input_state;
    create_pipeline_info.pInputAssemblyState = &m_input_assembly;
    create_pipeline_info.pViewportState      = &m_viewport_state;
    create_pipeline_info.pRasterizationState = &m_rasterizer;
    create_pipeline_info.pMultisampleState   = &m_multisampling;
    create_pipeline_info.pColorBlendState    = &m_color_blending;
    create_pipeline_info.layout              = m_config_info.pipeline_layout;
    create_pipeline_info.renderPass          = render_pass;
    create_pipeline_info.subpass             = 0;

    // optional:
    create_pipeline_info.flags = vk::PipelineCreateFlags();
    create_pipeline_info.pTessellationState;
    create_pipeline_info.pDynamicState;
    create_pipeline_info.pDepthStencilState = &m_depth_stencil;
    create_pipeline_info.basePipelineHandle;
    create_pipeline_info.basePipelineIndex = -1;

    return create_pipeline_info;
}

void
PipelineCreater::createShaderStages(const ShaderManager& shader_manager)
{
    m_vertex_shader_module =
        shader_manager[m_config_info.vertex_shader_type].get();

    m_shader_stages.push_back(vertexShaderinfo(vk::ShaderStageFlagBits::eVertex,
                                               m_vertex_shader_module));

    m_fragment_shader_module =
        shader_manager[m_config_info.fragment_shader_type].get();

    m_shader_stages.push_back(vertexShaderinfo(
        vk::ShaderStageFlagBits::eFragment, m_fragment_shader_module));
}

vk::UniquePipeline
PipelineCreater::createPipeline(const ShaderManager& shader_manager,
                                const vk::RenderPass& render_pass,
                                const LogicalDevice& device)
{
    createShaderStages(shader_manager);

    auto&& res = device.get().createGraphicsPipelineUnique(
        nullptr, convertToVulkanConfigInfo(render_pass));

    if (res.result != vk::Result::eSuccess)
    {
        throw std::logic_error("Failed create pipeline");
    }
    return {std::move(res.value)};
}

vk::PipelineInputAssemblyStateCreateInfo
PipelineCreater::inputAssemblyState()
{
    vk::PipelineInputAssemblyStateCreateInfo input_assembly_info;
    input_assembly_info.flags    = vk::PipelineInputAssemblyStateCreateFlags();
    input_assembly_info.topology = vk::PrimitiveTopology::eTriangleList;
    return input_assembly_info;
}

vk::PipelineViewportStateCreateInfo
PipelineCreater::viewportState()
{
    m_viewport.x        = 0.0f;
    m_viewport.y        = 0.0f;
    m_viewport.width    = m_config_info.extent.width;
    m_viewport.height   = m_config_info.extent.height;
    m_viewport.minDepth = 0.0f;
    m_viewport.maxDepth = 1.0f;

    m_scissor.offset.x = 0.0f;
    m_scissor.offset.y = 0.0f;
    m_scissor.extent   = m_config_info.extent;

    vk::PipelineViewportStateCreateInfo viewport_state = {};
    viewport_state.flags         = vk::PipelineViewportStateCreateFlags();
    viewport_state.viewportCount = 1;
    viewport_state.pViewports    = &m_viewport;
    viewport_state.scissorCount  = 1;
    viewport_state.pScissors     = &m_scissor;

    return viewport_state;
}

vk::PipelineRasterizationStateCreateInfo
PipelineCreater::rasterizerState()
{
    vk::PipelineRasterizationStateCreateInfo rasterizer;

    rasterizer.flags            = vk::PipelineRasterizationStateCreateFlags();
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode             = vk::PolygonMode::eFill;
    rasterizer.lineWidth               = 1.0f;
    rasterizer.cullMode                = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace               = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable         = VK_FALSE;

    return rasterizer;
}

vk::PipelineMultisampleStateCreateInfo
PipelineCreater::multisamplingState()
{
    vk::PipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.flags = vk::PipelineMultisampleStateCreateFlags();
    multisampling.sampleShadingEnable  = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
    return multisampling;
}

vk::PipelineColorBlendStateCreateInfo
PipelineCreater::colorBlendState()
{
    m_color_blend_attachment.blendEnable = vk::True;
    m_color_blend_attachment.colorWriteMask =
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

    m_color_blend_attachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;

    m_color_blend_attachment.dstColorBlendFactor =
        vk::BlendFactor::eOneMinusSrcAlpha;

    m_color_blend_attachment.colorBlendOp        = vk::BlendOp::eAdd;
    m_color_blend_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
    m_color_blend_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
    m_color_blend_attachment.alphaBlendOp        = vk::BlendOp::eAdd;

    vk::PipelineColorBlendStateCreateInfo color_blending = {};
    color_blending.flags             = vk::PipelineColorBlendStateCreateFlags();
    color_blending.logicOpEnable     = vk::False;
    color_blending.logicOp           = vk::LogicOp::eCopy;
    color_blending.attachmentCount   = 1;
    color_blending.pAttachments      = &m_color_blend_attachment;
    color_blending.blendConstants[0] = 0.0f;
    color_blending.blendConstants[1] = 0.0f;
    color_blending.blendConstants[2] = 0.0f;
    color_blending.blendConstants[3] = 0.0f;

    return color_blending;
}

vk::PipelineShaderStageCreateInfo
PipelineCreater::vertexShaderinfo(vk::ShaderStageFlagBits flag,
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
PipelineCreater::vertexInputState()
{
    vk::PipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.flags = vk::PipelineVertexInputStateCreateFlags();

    vertex_input_info.vertexBindingDescriptionCount = 1;
    vertex_input_info.pVertexBindingDescriptions =
        &m_config_info.vertex_binding_description;

    vertex_input_info.vertexAttributeDescriptionCount =
        m_config_info.vertex_attribute_descriptions.size();
    vertex_input_info.pVertexAttributeDescriptions =
        m_config_info.vertex_attribute_descriptions.data();

    return vertex_input_info;
}

vk::PipelineDepthStencilStateCreateInfo
PipelineCreater::depthStencil()
{
    vk::PipelineDepthStencilStateCreateInfo depth_stencil_info{};

    depth_stencil_info.depthTestEnable   = m_config_info.depth_test_enable;
    depth_stencil_info.depthWriteEnable  = vk::True;
    depth_stencil_info.depthCompareOp    = vk::CompareOp::eLess;
    depth_stencil_info.stencilTestEnable = vk::True;

    return depth_stencil_info;
}
} // namespace ars_graphics
