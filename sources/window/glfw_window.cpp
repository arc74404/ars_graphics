#include "glfw_window.hpp"

#include <iostream>

namespace ars_graphics
{
GlfwWindow::~GlfwWindow()
{
    glfwTerminate();
}

void
GlfwWindowDeleter::operator()(GLFWwindow* window) const
{
    if (window)
    {
        glfwDestroyWindow(window);
    }
}

GlfwWindow::GlfwWindow(const GlfwWindowConfigInfo& config_info)
    : m_config(config_info)
{
    if (glfwInit() == GLFW_FALSE)
    {
        throw std::runtime_error("Failed init glfw");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window.reset(glfwCreateWindow(m_config.m_width, m_config.m_height,
                                    m_config.m_title.c_str(), nullptr,
                                    nullptr));

    if (m_window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
}

vk::SurfaceKHR
GlfwWindow::createSurface(const vk::Instance& instance)
{
    VkSurfaceKHR surface{};

    glfwCreateWindowSurface(instance, m_window.get(), nullptr, &surface);

    return surface;
}
} // namespace ars_graphics
