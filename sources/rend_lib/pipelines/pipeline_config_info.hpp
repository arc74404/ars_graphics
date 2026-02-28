#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "../shaders/shader_manager.hpp"

namespace ars_graphics
{

struct PipelineConfigInfo final
{
    vk::VertexInputBindingDescription vertex_binding_description;

    std::vector<vk::VertexInputAttributeDescription>
        vertex_attribute_descriptions;

    std::vector<vk::DynamicState> dynamic_states;

    vk::ShaderModule vertex_shader_module;
    vk::ShaderModule fragment_shader_module;

    vk::Bool32 depth_test_enable = vk::False;
    vk::PipelineLayout pipeline_layout;

    vk::Extent2D extent;
};
} // namespace ars_graphics
