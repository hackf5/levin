#pragma once

#include <memory>

#include <vma/vk_mem_alloc.h>

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

        VmaAllocator m_allocator;

        VkQueue m_graphics_queue;
        VkQueue m_present_queue;
        VkQueue m_transfer_queue;

        std::shared_ptr<WindowComponents> m_window_components;

        void init_device(bool enable_validation_layers);
        void init_allocator();
        void init_queues();
    public:
        DeviceComponents(
            const std::shared_ptr<WindowComponents> &window_components,
            bool enable_validation_layers);
        ~DeviceComponents();

        static const uint32_t frames_in_flight = 2;

        vkb::Device& get_device() { return m_device; }

        VkQueue get_graphics_queue() { return m_graphics_queue; }

        VkQueue get_present_queue() { return m_present_queue; }

        VkQueue get_transfer_queue() { return m_transfer_queue; }

        void wait_idle() { vkDeviceWaitIdle(m_device.device); }

        VmaAllocator get_allocator() { return m_allocator; }
    };
}