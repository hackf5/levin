#include "window_components.h"

#include <stdexcept>

#include <spdlog/spdlog.h>

using namespace levin;

WindowComponents::WindowComponents(int width, int height, const std::string &title)
    : m_width(width), m_height(height)
{
    spdlog::info("Creating Window Components");

    if (glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
}

WindowComponents::~WindowComponents()
{
    spdlog::info("Destroying Window Components");
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void WindowComponents::framebuffer_resize_callback(GLFWwindow *window, int width, int height)
{
    auto app = reinterpret_cast<WindowComponents *>(glfwGetWindowUserPointer(window));

    if (app->m_framebuffer_resize_callback)
    {
        app->m_framebuffer_resize_callback(width, height);
    }
}

void WindowComponents::create_window_surface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface");
    }
}

void WindowComponents::register_framebuffer_resize_callback(framebuffer_resize_callback_t callback)
{
    m_framebuffer_resize_callback = callback;
}