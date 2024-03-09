#pragma once

#include <memory>

#include "window_components.h"
#include "device_components.h"
#include "render_pass_components.h"
#include "swapchain_components.h"
#include "graphics_pipeline_components.h"
#include "graphics_commands.h"
#include "buffer_components.h"

namespace levin
{
    class VulkanEngine
    {
    private:
        std::shared_ptr<WindowComponents> m_window_components;
        std::shared_ptr<DeviceComponents> m_device_components;

        RenderPassComponents m_render_pass;
        std::unique_ptr<SwapchainComponents> m_swapchain;
        GraphicsPipelineComponents m_graphics_pipeline;
        GraphicsCommands m_graphics_commands;

        BufferComponents m_buffer_components;

        uint32_t m_current_frame = 0;

        uint32_t m_image_index;

        void next_frame()
        {
            m_current_frame = (m_current_frame + 1) % GraphicsCommands::frames_in_flight;
        }

        void recreate_swapchain();

        void record_command_buffer();

        void draw_frame();

    public:
        VulkanEngine(
            const std::shared_ptr<WindowComponents> &window_components,
            const std::shared_ptr<DeviceComponents> &device_components);

        void run();
    };
}