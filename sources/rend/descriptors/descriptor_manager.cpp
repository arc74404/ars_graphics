#include "descriptor_manager.hpp"

#include "../global_settings.hpp"

namespace ars_graphics
{

namespace
{
std::vector<DescriptorBindingData>
defaultVertexShaderBindings()
{
    std::vector<DescriptorBindingData> result(1);
    result[0].count         = 1;
    result[0].binding_index = settings::bindings::uniform_buffer_binding;
    result[0].stage         = vk::ShaderStageFlagBits::eVertex;
    result[0].type          = vk::DescriptorType::eUniformBuffer;

    return result;
}

std::vector<DescriptorBindingData>
defaultMaterialShaderBindings()
{
    std::vector<DescriptorBindingData> result(6);

    // Binding 0: Albedo map
    result[0].count         = 1;
    result[0].binding_index = settings::bindings::material_shader_binding;
    result[0].stage         = vk::ShaderStageFlagBits::eFragment;
    result[0].type          = vk::DescriptorType::eCombinedImageSampler;

    // Binding 1: Normal map
    result[1].count         = 1;
    result[1].binding_index = settings::bindings::material_shader_binding + 1;
    result[1].stage         = vk::ShaderStageFlagBits::eFragment;
    result[1].type          = vk::DescriptorType::eCombinedImageSampler;

    // Binding 2: Metallic-Roughness map
    result[2].count         = 1;
    result[2].binding_index = settings::bindings::material_shader_binding + 2;
    result[2].stage         = vk::ShaderStageFlagBits::eFragment;
    result[2].type          = vk::DescriptorType::eCombinedImageSampler;

    // Binding 3: AO map
    result[3].count         = 1;
    result[3].binding_index = settings::bindings::material_shader_binding + 3;
    result[3].stage         = vk::ShaderStageFlagBits::eFragment;
    result[3].type          = vk::DescriptorType::eCombinedImageSampler;

    // Binding 4: Emissive map
    result[4].count         = 1;
    result[4].binding_index = settings::bindings::material_shader_binding + 4;
    result[4].stage         = vk::ShaderStageFlagBits::eFragment;
    result[4].type          = vk::DescriptorType::eCombinedImageSampler;

    // Binding 5: other data
    result[5].count         = 1;
    result[5].binding_index = settings::bindings::material_shader_binding + 5;
    result[5].stage         = vk::ShaderStageFlagBits::eFragment;
    result[5].type          = vk::DescriptorType::eUniformBuffer;
    return result;
}
} // namespace

DescriptorManager::DescriptorManager(const LogicalDevice& device)
{
    addAllocator(DescriptorSetLayoutType::UBO_AND_STORAGE, device,
                 defaultVertexShaderBindings(), 100, 100);

    addAllocator(DescriptorSetLayoutType::MATERIAL, device,
                 defaultMaterialShaderBindings(), 100, 100);
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
