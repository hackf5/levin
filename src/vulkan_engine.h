#pragma once

#include <memory>

#include "vulkan_context.h"

namespace levin
{
    class VulkanEngine
    {
    private:
        std::unique_ptr<VulkanContext> m_context;

        void load_vertexes();
        void load_indexes();

        void draw_frame();
        void recreate_swapchain();
        void update_uniform_buffer();
        void record_command_buffer();

    public:
        VulkanEngine(std::unique_ptr<VulkanContext> context);
        VulkanEngine(const VulkanEngine &) = delete;

        void run();
    };
}