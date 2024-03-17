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

        VulkanContextBuilder &configure_graphics_commands();

        VulkanContextBuilder &configure_descriptor_pool();

        VulkanContextBuilder &configure_descriptor_set_layout();

        VulkanContextBuilder &configure_vertex_buffer(VkDeviceSize size);

        VulkanContextBuilder &configure_index_buffer(VkDeviceSize size);

        VulkanContextBuilder &configure_uniform_buffers(VkDeviceSize size);

        VulkanContextBuilder &configure_uniform_buffer_descriptor_set(VkDeviceSize size);

        VulkanContextBuilder &configure_shader_modules();

        VulkanContextBuilder &configure_swapchain();

        VulkanContextBuilder &configure_render_pass();

        VulkanContextBuilder &configure_framebuffers();

        VulkanContextBuilder &configure_graphics_pipeline();

        std::unique_ptr<VulkanContext> build();
    };
}