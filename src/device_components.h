#pragma once

#include <memory>

#include "VkBootstrap.h"

#include "window_components.h"

namespace levin
{
    struct DeviceComponents
    {
        VkSurfaceKHR surface;

        vkb::Instance instance;
        vkb::Device device;

        VkQueue graphics_queue;
        VkQueue present_queue;
        VkQueue transfer_queue;

        DeviceComponents(
            const std::shared_ptr<WindowComponents>& window_components,
            bool enable_validation_layers);
        ~DeviceComponents();

    private:
        std::shared_ptr<WindowComponents> m_window_components;

        void init_device(bool enable_validation_layers);
        void init_queues();
    };
}