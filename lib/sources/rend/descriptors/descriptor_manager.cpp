#include "descriptor_manager.hpp"

namespace ars_graphics
{

DescriptorManager::DescriptorManager(const LogicalDevice& device)
{
    std::vector<DescriptorBindingData> default_vertex_shader_bindings_data(2);
    default_vertex_shader_bindings_data[0].count         = 1;
    default_vertex_shader_bindings_data[0].binding_index = 0;
    default_vertex_shader_bindings_data[0].stage =
        vk::ShaderStageFlagBits::eVertex;
    default_vertex_shader_bindings_data[0].type =
        vk::DescriptorType::eUniformBuffer;

    default_vertex_shader_bindings_data[1].count         = 1;
    default_vertex_shader_bindings_data[1].binding_index = 1;
    default_vertex_shader_bindings_data[1].stage =
        vk::ShaderStageFlagBits::eVertex;
    default_vertex_shader_bindings_data[1].type =
        vk::DescriptorType::eStorageBuffer;

    addAllocator(DescriptorSetLayoutType::UBO_x_STORAGE, device,
                 default_vertex_shader_bindings_data, 100, 100);

    std::vector<DescriptorBindingData> default_fragment_shader_bindings_data(1);
    default_fragment_shader_bindings_data[0].count         = 1;
    default_fragment_shader_bindings_data[0].binding_index = 0;
    default_fragment_shader_bindings_data[0].stage =
        vk::ShaderStageFlagBits::eFragment;
    default_fragment_shader_bindings_data[0].type =
        vk::DescriptorType::eCombinedImageSampler;

    addAllocator(DescriptorSetLayoutType::COMBINED_IMAGE_SAMPLER, device,
                 default_fragment_shader_bindings_data, 100, 100);
}

void
DescriptorManager::addAllocator(
    DescriptorSetLayoutType sh_type,
    const LogicalDevice& device,
    const std::vector<DescriptorBindingData>& bindings,
    uint32_t max_sets,
    uint32_t desc_count)
{
    DescriptorAllocator temp{device, bindings, max_sets, desc_count};
    m_descriptor_allocator_storage.emplace(sh_type, std::move(temp));
}

const DescriptorAllocator&
DescriptorManager::getAllocator(DescriptorSetLayoutType sh_type) const
{
    return m_descriptor_allocator_storage.at(sh_type);
}
}; // namespace ars_graphics
