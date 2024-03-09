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
        CommandFactory m_command_factory;

        VkQueue m_graphics_queue;
        VkCommandPool m_command_pool;
        std::vector<VkCommandBuffer> m_command_buffers;
        std::vector<VkSemaphore> m_image_available;
        std::vector<VkSemaphore> m_render_finished;
        std::vector<VkFence> m_in_flight_fences;

        static VkCommandPool create_command_pool(CommandFactory &command_factory);

        static std::vector<VkCommandBuffer> create_command_buffers(
            CommandFactory &command_factory,
            VkCommandPool command_pool);

    public:
        GraphicsCommands(const std::shared_ptr<DeviceComponents> &device_components);

        GraphicsResult acquire_next_image(uint32_t frame, VkSwapchainKHR swapchain, uint32_t &image_index);

        void reset_command_buffer(uint32_t frame);
        VkCommandBuffer begin_command_buffer(uint32_t frame);
        void end_command_buffer(uint32_t frame);
        void submit_command_buffer(uint32_t frame);

        GraphicsResult present(uint32_t frame, VkSwapchainKHR swapchain, uint32_t image_index);
    };
}