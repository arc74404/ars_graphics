#include "material.hpp"

namespace ars_graphics
{

NonUpdatebleMaterial::NonUpdatebleMaterial(
    const InterfaceMaterialSettupper& settuper)
{
    auto&& result = settuper.generateSets();

    if (false == result.has_value())
    {
        // error handling
    }

    auto&& [resources, sets_to_bind] = result.value();

    m_proxy_to_bind.resize(m_sets_to_bind.size());

    for (auto&& set : m_sets_to_bind)
    {
        m_proxy_to_bind.push_back(set.get());
    }
}

void
NonUpdatebleMaterial::bind(vk::CommandBuffer cmd,
                           vk::PipelineLayout layout) const
{
    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, layout, 1u,
                           m_proxy_to_bind.size(), m_proxy_to_bind.data(), 0,
                           nullptr);
}

} // namespace ars_graphics
