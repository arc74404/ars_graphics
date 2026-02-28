#pragma once
#include <my_render.hpp>
#include <vector>

namespace ars_graphics
{
std::vector<RenderPassUserConfigInfo>
createRpConfigs();
std::vector<ShaderConfigInfo>
createShConfigs();

std::unique_ptr<RenderPipeline>
createRenderPipeline();
} // namespace ars_graphics
