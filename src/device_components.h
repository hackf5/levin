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
            WindowComponents &window_components,
            bool enable_validation_layers);
        ~DeviceComponents();

    private:
        DeviceComponents() {}

        void init_device(WindowComponents &window_components, bool enable_validation_layers);

        void init_queues();
    };
}