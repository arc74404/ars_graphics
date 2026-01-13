#pragma once

#include <map>

#include "buffers/gpu_buffer.hpp"
#include "buffers/index_buffer.hpp"
#include "model/model_manager.hpp"
#include "render/renderable.hpp"

namespace ars_graphics
{

class Renderer;

class Scene final : public Renderable
{
public:
    Scene();

    void addModel(const Model* new_model);

    RenderInfo calculateRenderInfo(
        const LogicalDevice& logical_device,
        const PhysicalDevice& physical_device) const override;

private:
    std::map<const Model*, size_t> m_models;
};
} // namespace ars_graphics
