#include "glfw_event_manager.hpp"

#include <iostream>

#include "key.hpp"

namespace ars_graphics
{

GlfwEventManager::GlfwEventManager()
{
    m_events.reserve(10);
}

// void
// GlfwEventManager::keyCallback(GLFWwindow* window,
//                               int key,
//                               int scancode,
//                               int action,
//                               int mods)
// {
//     GlfwEventManager* d =
//         static_cast<GlfwEventManager*>(glfwGetWindowUserPointer(window));
//     d->m_events.emplace_back(std::make_unique<KeyEvent>(
//         convertGlfwKey(key), convertGlfwStatus(action)));

//     std::cout << "Callback\n";
// }

void
GlfwEventManager::keyEvents(GLFWwindow* window)
{
    for (int i = 0; i <= GLFW_KEY_LAST; ++i)
    {
        int key_status = glfwGetKey(window, i);

        if (key_status == GLFW_PRESS)
        {
            auto&& k = convertGlfwKey(i);

            if (false == k.has_value())
            {
                continue;
            }
            m_events.emplace_back(std::make_unique<KeyEvent>(
                k.value(), convertGlfwStatus(GLFW_PRESS)));
        }
    }
}
void
GlfwEventManager::mouseEvents(GLFWwindow* window)
{
    glm::vec<2, double> new_pos;
    glfwGetCursorPos(window, &(new_pos.x), &(new_pos.y));

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        m_events.emplace_back(std::make_unique<MoveMouseEvent>(
            new_pos - m_mouse_info.last_position));
    }
    m_mouse_info.last_position = new_pos;
}

const std::vector<EventPtr>&
GlfwEventManager::pollEvents(GLFWwindow* window)
{
    m_events.clear();
    glfwPollEvents();

    if (glfwWindowShouldClose(window))
    {
        std::cout << "After callback\n";
    }

    keyEvents(window);
    mouseEvents(window);

    return m_events;
}

void
GlfwEventManager::closeWindowCallBack(GLFWwindow* w)
{
    GlfwEventManager* d =
        static_cast<GlfwEventManager*>(glfwGetWindowUserPointer(w));
    d->m_events.emplace_back(std::make_unique<CloseWindowEvent>());

    std::cout << "CALLLBACK\n";
}

void
GlfwEventManager::setupCallBacks(GLFWwindow* window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetWindowCloseCallback(window, closeWindowCallBack);
    // glfwSetKeyCallback(window, keyCallback);
}
} // namespace ars_graphics
