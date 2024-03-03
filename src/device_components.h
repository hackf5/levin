#pragma once

#include <memory>

#include "VkBootstrap.h"

#include "window_components.h"

namespace levin
{
    class DeviceComponents
    {
    private:
        VkSurfaceKHR m_surface;

        vkb::Instance m_instance;
        vkb::Device m_device;

        VkQueue m_graphics_queue;
        VkQueue m_present_queue;
        VkQueue m_transfer_queue;

        std::shared_ptr<WindowComponents> m_window_components;

        void init_device(bool enable_validation_layers);
        void init_queues();
    public:
        DeviceComponents(
            const std::shared_ptr<WindowComponents> &window_components,
            bool enable_validation_layers);
        ~DeviceComponents();

        vkb::Device& get_device() { return m_device; }
    };
}