#include "glfw_window.h"

#include <iostream>
#include <stdexcept>

using namespace levin;

GLFWWindow::GLFWWindow(int width, int height, const std::string &title)
    : m_width(width), m_height(height), m_framebufferResized(false)
{
    std::cout << "Creating GLFW Window" << std::endl;

    if (glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
}

GLFWWindow::~GLFWWindow()
{
    std::cout << "Destroying GLFW Window" << std::endl;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GLFWWindow::framebuffer_resize_callback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<GLFWWindow *>(glfwGetWindowUserPointer(window));
    app->m_framebufferResized = true;
}

void GLFWWindow::create_window_surface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface");
    }
}