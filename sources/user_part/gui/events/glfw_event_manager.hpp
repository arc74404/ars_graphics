#pragma once

#include <memory>
#include <vector>

#include "event.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace ars_graphics
{

struct MouseInfo
{
    glm::vec<2, double> last_position;
};

class GlfwEventManager final
{
public:
    GlfwEventManager();
    const std::vector<EventPtr>& pollEvents(GLFWwindow* window);

    void setupCallBacks(GLFWwindow* window);

private:
    static void closeWindowCallBack(GLFWwindow* w);

    static void keyCallback(GLFWwindow* window,
                            int key,
                            int scancode,
                            int action,
                            int mods);

    void keyEvents(GLFWwindow* window);
    void mouseEvents(GLFWwindow* window);

    static void framebufferResizeCallback(GLFWwindow* window,
                                          int width,
                                          int height)
    {
        auto self =
            static_cast<GlfwEventManager*>(glfwGetWindowUserPointer(window));
        if (self)
        {
            self->m_window_resized = true;
        }
    }
    bool m_window_resized = false;

    MouseInfo m_mouse_info;

    std::vector<EventPtr> m_events;
};
} // namespace ars_graphics
