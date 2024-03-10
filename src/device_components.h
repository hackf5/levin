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

        operator const vkb::Device&() const { return m_device; }

        operator VkDevice() const { return m_device.device; }

        VkQueue graphics_queue() const { return m_graphics_queue; }
        uint32_t graphics_queue_index() const { return m_device.get_queue_index(vkb::QueueType::graphics).value(); }

        VkQueue present_queue() const { return m_present_queue; }
        uint32_t present_queue_index() const { return m_device.get_queue_index(vkb::QueueType::present).value(); }

        VkQueue transfer_queue() const { return m_transfer_queue; }
        uint32_t transfer_queue_index() const { return m_device.get_queue_index(vkb::QueueType::transfer).value(); }

        void wait_idle() const { vkDeviceWaitIdle(m_device); }

        VmaAllocator allocator() const { return m_allocator; }
    };
}