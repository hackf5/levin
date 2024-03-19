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

        VulkanContextBuilder &add_window(int width, int height, const std::string &title);

        VulkanContextBuilder &add_device(bool enable_validation_layers = true);

        VulkanContextBuilder &add_transfer_queue();

        VulkanContextBuilder &add_graphics_queue();

        VulkanContextBuilder &add_descriptor_pool();

        VulkanContextBuilder &add_descriptor_set_layout();

        VulkanContextBuilder &add_model();

        VulkanContextBuilder &add_camera();

        VulkanContextBuilder &add_swapchain();

        VulkanContextBuilder &add_render_pass();

        VulkanContextBuilder &add_framebuffers();

        VulkanContextBuilder &add_graphics_pipeline();

        VulkanContextBuilder &add_gui();

        std::unique_ptr<VulkanContext> build();
    };
}