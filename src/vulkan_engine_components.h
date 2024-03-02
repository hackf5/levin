#pragma once

#include "glfw_window.h"

#include "VkBootstrap.h"

namespace levin
{
    struct VulkanEngineComponents
    {
        VkSurfaceKHR surface;

        vkb::Instance instance;
        vkb::Device device;
        vkb::Swapchain swapchain;

        VkQueue graphics_queue;

        VulkanEngineComponents(
            GLFWWindow &window,
            bool enable_validation_layers);
        ~VulkanEngineComponents();

    private:
        VulkanEngineComponents() {}

        void init_device(GLFWWindow &window, bool enable_validation_layers);

        void init_swapchain();
    };
}