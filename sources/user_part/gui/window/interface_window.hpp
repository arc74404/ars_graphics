#pragma once

#include <my_render.hpp>

#include "../events/event.hpp"

namespace user_part
{

class Renderer;

struct WindowConfigInfo final
{
    std::string m_title = "Vulkan Application";
    int m_width         = 800;
    int m_height        = 600;
};

enum class EventType
{
    NONE,
    CLOSE
};

class IWindow : public ars_graphics::SurfaceCreater
{
public:
    virtual ~IWindow() = default;

    IWindow(const WindowConfigInfo& config_info) : m_config_info(config_info)
    {
    }

    uint32_t getWidth() const
    {
        return m_config_info.m_width;
    }

    uint32_t getHeight() const
    {
        return m_config_info.m_height;
    }
    virtual void close() = 0;

    virtual const std::vector<EventPtr>& pollEvents() = 0;

    virtual bool IsOpen() const = 0;

private:
    WindowConfigInfo m_config_info;
};
} // namespace user_part
