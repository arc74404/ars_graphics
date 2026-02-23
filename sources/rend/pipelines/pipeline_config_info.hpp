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

    ShaderType vertex_shader_type;
    ShaderType fragment_shader_type;

    vk::Bool32 depth_test_enable = vk::False;
    vk::PipelineLayout pipeline_layout;

    vk::Extent2D extent;
};
} // namespace ars_graphics
