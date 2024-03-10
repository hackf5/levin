#pragma once

#include <memory>
#include <vector>

#include "VkBootstrap.h"

#include "device_components.h"

#include "command_factory.h"

namespace levin
{
    enum class GraphicsResult
    {
        Success,
        RecreateSwapchain,
    };

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

    public:
        GraphicsCommands(const DeviceComponents &device_components);
        GraphicsCommands(const GraphicsCommands &) = delete;

        uint32_t image_index() const { return m_image_index; }

        GraphicsResult acquire_next_image(uint32_t frame, VkSwapchainKHR swapchain);

        VkCommandBuffer begin(uint32_t frame) const;
        void end_and_submit(uint32_t frame) const;

        GraphicsResult present(uint32_t frame, VkSwapchainKHR swapchain) const;
    };
}