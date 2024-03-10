#pragma once

#include <memory>
#include <vector>

#include "window_components.h"
#include "device_components.h"
#include "buffer_transfer_queue.h"
#include "descriptor_components.h"
#include "render_pass_components.h"
#include "swapchain_components.h"
#include "framebuffer_components.h"
#include "graphics_pipeline_components.h"
#include "graphics_commands.h"
#include "buffer.h"
#include "vertex.h"
#include "descriptor_set_components.h"

namespace levin
{
    class VulkanContext
    {
    private:
        std::unique_ptr<WindowComponents> m_window;
        std::unique_ptr<DeviceComponents> m_device;
        std::unique_ptr<BufferTransferQueue> m_transfer_queue;
        std::unique_ptr<GraphicsCommands> m_graphics_commands;
        std::unique_ptr<DescriptorComponents> m_descriptor_components;
        std::unique_ptr<BufferGPU> m_vertex_buffer;
        std::unique_ptr<BufferGPU> m_index_buffer;
        std::vector<std::unique_ptr<BufferCPUtoGPU>> m_uniform_buffers;
        std::unique_ptr<UniformBufferDescriptorSet> m_uniform_buffer_descriptor_set;
        std::unique_ptr<SwapchainComponents> m_swapchain;
        std::unique_ptr<RenderPassComponents> m_render_pass;
        std::unique_ptr<FramebufferComponents> m_framebuffers;
        std::unique_ptr<GraphicsPipelineComponents> m_graphics_pipeline;

        uint32_t m_current_frame = 0;

    public:
        static const uint32_t max_frames_in_flight = 2;

        VulkanContext() = default;

        VulkanContext(const VulkanContext &) = delete;

        uint32_t current_frame() const { return m_current_frame; }

        WindowComponents &window() { return *m_window; }

        const DeviceComponents &device() const { return *m_device; }

        const BufferTransferQueue &transfer_queue() const { return *m_transfer_queue; }

        GraphicsCommands &graphics_commands() { return *m_graphics_commands; }
        const GraphicsCommands &graphics_commands() const { return *m_graphics_commands; }

        const DescriptorComponents &descriptor_components() const { return *m_descriptor_components; }

        const BufferGPU &vertex_buffer() const { return *m_vertex_buffer; }

        const BufferGPU &index_buffer() const { return *m_index_buffer; }

        const BufferCPUtoGPU &uniform_buffer(uint32_t index) const { return *m_uniform_buffers[index]; }
        BufferCPUtoGPU &uniform_buffer(uint32_t index) { return *m_uniform_buffers[index]; }

        const UniformBufferDescriptorSet &uniform_buffer_descriptor_set() const
        {
            return *m_uniform_buffer_descriptor_set;
        }

        const SwapchainComponents &swapchain() const { return *m_swapchain; }

        const RenderPassComponents &render_pass() const { return *m_render_pass; }

        const FramebufferComponents &framebuffers() const { return *m_framebuffers; }

        const GraphicsPipelineComponents &graphics_pipeline() const { return *m_graphics_pipeline; }

        void next_frame()
        {
            m_current_frame = (m_current_frame + 1) % DeviceComponents::frames_in_flight;
        }

        friend class VulkanContextBuilder;
    };

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

        VulkanContextBuilder &configure_vertex_buffer(VkDeviceSize size);

        VulkanContextBuilder &configure_index_buffer(VkDeviceSize size);

        VulkanContextBuilder &configure_uniform_buffers(VkDeviceSize size);

        VulkanContextBuilder &configure_uniform_buffer_descriptor_set(VkDeviceSize size);

        VulkanContextBuilder &configure_swapchain();

        VulkanContextBuilder &configure_render_pass();

        VulkanContextBuilder &configure_framebuffers();

        VulkanContextBuilder &configure_graphics_pipeline();

        std::unique_ptr<VulkanContext> build();
    };
}