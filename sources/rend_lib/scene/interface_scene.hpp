#pragma once

#include "../render/render_setuper.hpp"

namespace ars_graphics
{
template <typename Derived>
class IResourceScene
{
public:
    IResourceScene(const RenderSetuper& setuper) : m_setuper(setuper)
    {
    }

    void bindDescriptors(vk::CommandBuffer cmd,
                         vk::PipelineLayout pipeline_layout,
                         uint32_t frame_index) const
    {
        static_cast<Derived*>(this)->bindDescriptorsImpl(cmd, pipeline_layout,
                                                         frame_index);
    }

protected:
    const RenderSetuper& m_setuper;
};
} // namespace ars_graphics
