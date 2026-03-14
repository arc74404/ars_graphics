#pragma once

#include <string>

#include "../events/glfw_event_manager.hpp"

#include "interface_window.hpp"

namespace user_part
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

    const std::vector<EventPtr>& pollEvents() override;

    void close() override;

    bool IsOpen() const override;

    ~GlfwWindow();

private:
    GlfwEventManager m_event_manager;

    std::unique_ptr<GLFWwindow, GlfwWindowDeleter> m_window;
};
} // namespace user_part
