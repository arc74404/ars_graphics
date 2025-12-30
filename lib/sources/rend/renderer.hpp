#pragma once
#include <iostream>

namespace ars_graphics
{
struct RendererConfigInfo final
{
};

class Renderer final
{
public:
    Renderer(const RendererConfigInfo& config_info);

private:
    RendererConfigInfo m_config_info;
};
}; // namespace ars_graphics
