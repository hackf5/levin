#pragma once

#include "vulkan_context.h"

namespace levin
{
    class VulkanContextBuilder
    {
    private:
        std::unique_ptr<VulkanContext> m_context;

    public:
        VulkanContextBuilder(): m_context(std::make_unique<VulkanContext>()) {}

        VulkanContextBuilder(std::unique_ptr<VulkanContext> context): m_context(std::move(context)) {}

        VulkanContextBuilder &configure_window(int width, int height, const std::string &title);

        VulkanContextBuilder &configure_device(bool enable_validation_layers = true);

        VulkanContextBuilder &configure_transfer_queue(size_t command_buffer_count = 1);

        VulkanContextBuilder &configure_graphics_queue();

        VulkanContextBuilder &configure_descriptor_pool();

        VulkanContextBuilder &configure_descriptor_set_layout();

        VulkanContextBuilder &configure_model();

        VulkanContextBuilder &configure_swapchain();

        VulkanContextBuilder &configure_render_pass();

        VulkanContextBuilder &configure_framebuffers();

        VulkanContextBuilder &configure_graphics_pipeline();

        std::unique_ptr<VulkanContext> build();
    };
}