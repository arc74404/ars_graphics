#pragma once

#include <string>

#include "interface_window.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace ars_graphics
{

struct GlfwWindowDeleter
{
    void operator()(GLFWwindow* window) const;
};

struct GlfwWindowConfigInfo final
{
    WindowConfigInfo base_config_info;
};

class GlfwWindow final : public IWindow
{
public:
    GlfwWindow() = default;
    GlfwWindow(const GlfwWindowConfigInfo& config_info);

    vk::SurfaceKHR createSurface(const vk::Instance& instance) override;

    EventType pollEvents() override;

    ~GlfwWindow();

private:
    bool should_clode = false;

    std::unique_ptr<GLFWwindow, GlfwWindowDeleter> m_window;
};
} // namespace ars_graphics
