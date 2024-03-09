#pragma once

#include <memory>

#include "window_components.h"
#include "device_components.h"
#include "buffer_transfer_queue.h"
#include "buffer.h"
#include "render_pass_components.h"
#include "swapchain_components.h"
#include "graphics_pipeline_components.h"
#include "graphics_commands.h"

namespace levin
{
    class VulkanEngine
    {
    private:
        std::shared_ptr<WindowComponents> m_window_components;
        std::shared_ptr<DeviceComponents> m_device_components;
        std::shared_ptr<BufferTransferQueue> m_transfer_queue;

        RenderPassComponents m_render_pass;
        std::unique_ptr<SwapchainComponents> m_swapchain;
        GraphicsPipelineComponents m_graphics_pipeline;
        GraphicsCommands m_graphics_commands;

        std::unique_ptr<BufferGPU> m_vertex_buffer;
        std::unique_ptr<BufferGPU> m_index_buffer;
        std::unique_ptr<BufferCPUtoGPU> m_uniform_buffer;

        uint32_t m_current_frame = 0;
        uint32_t m_image_index;

        void draw_frame();

        void recreate_swapchain();

        void record_command_buffer();

        void next_frame()
        {
            m_current_frame = (m_current_frame + 1) % GraphicsCommands::frames_in_flight;
        }

        void load_vertexes();
        void load_indexes();

    public:
        VulkanEngine(
            const std::shared_ptr<WindowComponents> &window_components,
            const std::shared_ptr<DeviceComponents> &device_components);

        void run();
    };
}