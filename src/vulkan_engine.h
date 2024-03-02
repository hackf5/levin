#pragma once

#include "glfw_window.h"

#include "VkBootstrap.h"

namespace levin
{
    class VulkanEngine
    {
    private:
        GLFWWindow *const m_window;
        bool m_enable_validation_layers;

        VkSurfaceKHR m_surface;
        vkb::Instance m_instance;
        vkb::Device m_device;

        VkQueue m_graphics_queue;
        VkQueue m_transfer_queue;

        void init_vulkan();

    public:
        VulkanEngine(
            GLFWWindow &window,
            bool enable_validation_layers);

        ~VulkanEngine();

        void run();
    };
}