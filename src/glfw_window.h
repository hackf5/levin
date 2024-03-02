#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace levin
{
    class GLFWWindow
    {
    private:
        int m_width;
        int m_height;
        bool m_framebufferResized;
        ::GLFWwindow *m_window;

        static void framebuffer_resize_callback(GLFWwindow *window, int width, int height);

    public:
        GLFWWindow(int width, int height, const std::string &title);
        ~GLFWWindow();

        void create_window_surface(VkInstance instance, VkSurfaceKHR *surface);
    };
}