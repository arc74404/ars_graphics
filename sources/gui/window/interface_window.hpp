#pragma once

#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

namespace ars_graphics
{

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

class IWindow
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

    virtual vk::SurfaceKHR createSurface(const vk::Instance& instance) = 0;

    virtual EventType pollEvents() = 0;

private:
    WindowConfigInfo m_config_info;
};
} // namespace ars_graphics
