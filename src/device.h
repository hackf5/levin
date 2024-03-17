#pragma once

#include <memory>

#include <vma/vk_mem_alloc.h>

#include "VkBootstrap.h"

#include "window.h"

namespace levin
{
    class Device
    {
    private:
        const Window &m_window;

        const vkb::Instance m_instance;
        const VkSurfaceKHR m_surface;
        const vkb::Device m_device;
        const VmaAllocator m_allocator;

        const VkQueue m_graphics_queue;
        const VkQueue m_present_queue;
        const VkQueue m_transfer_queue;

        vkb::Instance create_instance(bool enable_validation_layers);
        VkSurfaceKHR create_surface();
        vkb::Device create_device();
        VmaAllocator create_allocator();
        VkQueue create_queue(vkb::QueueType queue_type);

    public:
        Device(
            const Window &window,
            bool enable_validation_layers);
        Device(const Device &) = delete;

        ~Device();

        static const uint32_t max_frames_in_flight = 2;

        operator const vkb::Device &() const { return m_device; }

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