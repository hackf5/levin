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
        WindowComponents const * const m_window_components;

        VkSurfaceKHR m_surface;

        vkb::Instance m_instance;
        vkb::Device m_device;

        VmaAllocator m_allocator;

        VkQueue m_graphics_queue;
        VkQueue m_present_queue;
        VkQueue m_transfer_queue;

        void init_device(bool enable_validation_layers);
        void init_allocator();
        void init_queues();
    public:
        DeviceComponents(
            const WindowComponents &window_components,
            bool enable_validation_layers);
        DeviceComponents(const DeviceComponents &) = delete;

        ~DeviceComponents();

        static const uint32_t frames_in_flight = 2;

        const vkb::Device& get_device() const { return m_device; }

        VkQueue get_graphics_queue() const { return m_graphics_queue; }

        VkQueue get_present_queue() const { return m_present_queue; }

        VkQueue get_transfer_queue() const { return m_transfer_queue; }

        void wait_idle() const { vkDeviceWaitIdle(m_device); }

        VmaAllocator get_allocator() const { return m_allocator; }
    };
}