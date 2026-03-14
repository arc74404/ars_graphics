#pragma once
#include <memory>
#include <my_render.hpp>
#include <vector>

#include "../render_settings_structs/render_pipeline_3d.hpp"

namespace user_part
{
std::vector<ars_graphics::RenderPassUserConfigInfo>
createRpConfigs();
std::vector<ars_graphics::ShaderConfigInfo>
createShConfigs();

std::unique_ptr<ars_graphics::RenderPipeline3D>
createRenderPipeline();
} // namespace user_part
