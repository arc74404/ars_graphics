#pragma once

#include <my_render.hpp>

#include "../camera/interface_camera.hpp"

#include "resource_scene_3d.hpp"

namespace ars_graphics
{
template <>
class RenderPipeline<ResourceScene3D>
{
public:
    void updatePerFrameData(ResourceScene3D& scene, uint32_t frame_index) const;

    void beginCmd(vk::CommandBuffer cmd) const;

    void startRenderPass(vk::RenderPass renderpass,
                         vk::Framebuffer framebuffer,
                         const vk::Extent2D& extent) const;

    void setupScope(const vk::Extent2D& extent) const;

    void bindSharedData(const ResourceScene3D& scene) const;

    void drawPrimitives(const ResourceScene3D& scene) const;

    // user new funcs
    void bindCamera(ICamera* camera);

private:
    ICamera* m_camera;
};

void
RenderPipeline<ResourceScene3D>::bindCamera(ICamera* camera)
{
    m_camera = camera;
}

/////////////////////////////////////
void
RenderPipeline<ResourceScene3D>::updatePerFrameData(ResourceScene3D& scene,
                                                    uint32_t frame_index) const
{
    if (m_camera->needRecalculation())
    {
        scene.updateCamera(m_camera->recalculate(), frame_index);
    }
}

void
RenderPipeline<ResourceScene3D>::beginCmd(vk::CommandBuffer cmd) const
{
}

void
RenderPipeline<ResourceScene3D>::startRenderPass(
    vk::RenderPass renderpass,
    vk::Framebuffer framebuffer,
    const vk::Extent2D& extent) const
{
}

void
RenderPipeline<ResourceScene3D>::setupScope(const vk::Extent2D& extent) const
{
}

void
RenderPipeline<ResourceScene3D>::bindSharedData(
    const ResourceScene3D& scene) const
{
}

void
RenderPipeline<ResourceScene3D>::drawPrimitives(
    const ResourceScene3D& scene) const
{
}

} // namespace ars_graphics
