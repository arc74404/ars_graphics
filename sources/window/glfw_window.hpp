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
    std::string m_title = "Vulkan Application";
    int m_width         = 800;
    int m_height        = 600;
};

class GlfwWindow final : public IWindow
{
public:
    GlfwWindow() = default;
    GlfwWindow(const GlfwWindowConfigInfo& config_info);

    vk::SurfaceKHR createSurface(const vk::Instance& instance);

    ~GlfwWindow();

private:
    std::unique_ptr<GLFWwindow, GlfwWindowDeleter> m_window;
    GlfwWindowConfigInfo m_config;
};
} // namespace ars_graphics
