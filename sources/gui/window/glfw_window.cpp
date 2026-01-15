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

EventType
GlfwWindow::pollEvents()
{
    glfwPollEvents();

    if (should_clode)
    {
        return EventType::CLOSE;
    }
    return EventType::NONE;
}

GlfwWindow::GlfwWindow(const GlfwWindowConfigInfo& config_info)
    : IWindow(config_info.base_config_info)
{
    if (glfwInit() == GLFW_FALSE)
    {
        throw std::runtime_error("Failed init glfw");
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_window.reset(glfwCreateWindow(
        getWidth(), getHeight(), config_info.base_config_info.m_title.c_str(),
        nullptr, nullptr));

    if (m_window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwSetWindowUserPointer(m_window.get(), this);
    glfwSetWindowCloseCallback(m_window.get(),
                               [](GLFWwindow* w)
                               {
                                   GlfwWindow* d = static_cast<GlfwWindow*>(
                                       glfwGetWindowUserPointer(w));
                                   if (d)
                                   {
                                       d->should_clode = true;
                                   }
                               });
}

vk::SurfaceKHR
GlfwWindow::createSurface(const vk::Instance& instance)
{
    VkSurfaceKHR row_surface;
    glfwCreateWindowSurface(instance, m_window.get(), nullptr, &row_surface);
    return vk::SurfaceKHR{row_surface};
}
} // namespace ars_graphics
