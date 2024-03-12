#pragma once

#include <memory>
#include <vector>

#include "VkBootstrap.h"

#include "device_components.h"
#include "swapchain_components.h"
#include "framebuffer_components.h"

#include "command_factory.h"

namespace levin
{
    class GraphicsCommands
    {
    private:
        const DeviceComponents &m_device_components;

        CommandFactory m_command_factory;

        const VkQueue m_graphics_queue;
        const VkCommandPool m_command_pool;

        const std::vector<VkCommandBuffer> m_command_buffers;
        const std::vector<VkSemaphore> m_image_available;
        const std::vector<VkSemaphore> m_render_finished;
        const std::vector<VkFence> m_in_flight_fences;

        VkCommandPool create_command_pool();
        std::vector<VkCommandBuffer> create_command_buffers();

        uint32_t m_image_index = 0;
        uint32_t m_current_frame = 0;
        VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;

    public:
        GraphicsCommands(const DeviceComponents &device_components);
        GraphicsCommands(const GraphicsCommands &) = delete;

        VkFramebuffer prepare_framebuffer(
            uint32_t current_frame,
            VkSwapchainKHR swapchain,
            const FramebufferComponents& framebuffers);

        VkCommandBuffer begin_command() const;
        void submit_command() const;
        bool present_framebuffer();
    };
}