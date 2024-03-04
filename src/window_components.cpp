#include "window_components.h"

#include <stdexcept>

#include <spdlog/spdlog.h>

using namespace levin;

WindowComponents::WindowComponents(int width, int height, const std::string &title)
    : m_width(width), m_height(height)
{
    spdlog::info("Creating Window Components");

    try
    {
        if (glfwInit() != GLFW_TRUE)
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_window = glfwCreateWindow(m_width, m_height, title.c_str(), nullptr, nullptr);
    }
    catch (const std::exception &e)
    {
        this->~WindowComponents();
        throw;
    }
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

VkSurfaceKHR WindowComponents::create_window_surface(VkInstance instance)
{
    spdlog::info("Creating Window Surface");

    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface");
    }

    return surface;
}

void WindowComponents::register_framebuffer_resize_callback(framebuffer_resize_callback_t callback)
{
    m_framebuffer_resize_callback = callback;
}

void WindowComponents::wait_resize()
{
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }
}