#pragma once
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#include "../shaders/shader_manager.hpp"

namespace ars_graphics
{
struct PipelineConfigInfo final
{
    // vertex data
    vk::VertexInputBindingDescription vertex_binding_description;
    std::vector<vk::VertexInputAttributeDescription>
        vertex_attribute_descriptions;

    // layout
    vk::PipelineLayout pipeline_layout;

    // extent
    vk::Extent2D extent;

    // shaders types
    ShaderType vertex_shader_type;
    ShaderType fragment_shader_type;

    // flags
    vk::Bool32 depth_test_enable = vk::False;
};
} // namespace ars_graphics
