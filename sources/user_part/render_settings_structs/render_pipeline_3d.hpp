#pragma once

#include <my_render.hpp>

#include "../camera/interface_camera.hpp"

#include "instance_data.hpp"
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
    void bindCamera(user_part::ICamera* camera);

private:
    user_part::ICamera* m_camera;

    std::vector<user_part::ModelInstancingData> m_model_instancing_data;

    std::vector<user_part::MeshInstancingData> m_mesh_instancing_data;
};

inline void
RenderPipeline<ResourceScene3D>::bindCamera(user_part::ICamera* camera)
{
    m_camera = camera;
}

/////////////////////////////////////
inline void
RenderPipeline<ResourceScene3D>::updatePerFrameData(ResourceScene3D& scene,
                                                    uint32_t frame_index) const
{
    if (m_camera->needRecalculation())
    {
        scene.updateCamera(m_camera->recalculate(), frame_index);
    }
    scene.updateModelInstancing(m_model_instancing_data, frame_index);
    scene.updateMeshInstancing(m_mesh_instancing_data, frame_index);
}
inline void
RenderPipeline<ResourceScene3D>::beginCmd(vk::CommandBuffer cmd) const
{
}
inline void
RenderPipeline<ResourceScene3D>::startRenderPass(
    vk::RenderPass renderpass,
    vk::Framebuffer framebuffer,
    const vk::Extent2D& extent) const
{
}
inline void
RenderPipeline<ResourceScene3D>::setupScope(const vk::Extent2D& extent) const
{
}
inline void
RenderPipeline<ResourceScene3D>::bindSharedData(
    const ResourceScene3D& scene) const
{
}
inline void
RenderPipeline<ResourceScene3D>::drawPrimitives(
    const ResourceScene3D& scene) const
{
}

using RenderPipeline3D = RenderPipeline<ResourceScene3D>;

} // namespace ars_graphics
