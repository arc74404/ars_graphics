#pragma once

namespace ars_graphics
{
class InterfaceMaterialSettupper
{
public:
    ~InterfaceMaterialSettupper() = default;
    std::vector<vk::UniqueDescriptorSet> generateSets() const;

private:
};
} // namespace ars_graphics
