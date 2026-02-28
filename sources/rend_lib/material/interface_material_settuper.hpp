#pragma once

#include <optional>
#include <tuple>
#include <vector>

#include "../../libs_includes/vulkan.hpp"
#include "../buffers/uniform_buffer.hpp"

#include "material_resources.hpp"

namespace ars_graphics
{
class InterfaceMaterialSettupper
{
public:
    ~InterfaceMaterialSettupper() noexcept = default;
    std::optional<
        std::tuple<MaterialResources, std::vector<vk::UniqueDescriptorSet>>>
    generateSets() const;

private:
    virtual void setupUboBuffer() const = 0;
    virtual void setupTextures() const  = 0;
};
} // namespace ars_graphics
