#pragma once

#include <memory>
#include <vector>

#include "window_components.h"
#include "device_components.h"
#include "buffer_transfer_queue.h"
#include "graphics_commands.h"
#include "descriptor_pool_components.h"
#include "buffer.h"
#include "vertex.h"
#include "descriptor_set_components.h"
#include "shader_module_components.h"
#include "swapchain_components.h"
#include "render_pass_components.h"
#include "framebuffer_components.h"
#include "graphics_pipeline_components.h"

namespace levin
{
    class VulkanContext
    {
    private:
        std::unique_ptr<WindowComponents> m_window;
        std::unique_ptr<DeviceComponents> m_device;
        std::unique_ptr<BufferTransferQueue> m_transfer_queue;
        std::unique_ptr<GraphicsCommands> m_graphics_commands;
        std::unique_ptr<DescriptorPoolComponents> m_descriptor_pool;
        std::unique_ptr<BufferGPU> m_vertex_buffer;
        std::unique_ptr<BufferGPU> m_index_buffer;
        std::vector<std::unique_ptr<BufferCPUtoGPU>> m_uniform_buffers;
        std::unique_ptr<UniformBufferDescriptorSet> m_uniform_buffer_descriptor_set;
        std::unique_ptr<ShaderModuleComponents> m_shader_modules;
        std::unique_ptr<SwapchainComponents> m_swapchain;
        std::unique_ptr<RenderPassComponents> m_render_pass;
        std::unique_ptr<FramebufferComponents> m_framebuffers;
        std::unique_ptr<GraphicsPipelineComponents> m_graphics_pipeline;

    public:
        VulkanContext() = default;

        VulkanContext(const VulkanContext &) = delete;

        WindowComponents &window() { return *m_window; }

        const DeviceComponents &device() const { return *m_device; }

        const BufferTransferQueue &transfer_queue() const { return *m_transfer_queue; }

        GraphicsCommands &graphics_commands() { return *m_graphics_commands; }
        const GraphicsCommands &graphics_commands() const { return *m_graphics_commands; }

        const DescriptorPoolComponents &descriptor_components() const { return *m_descriptor_pool; }

        const BufferGPU &vertex_buffer() const { return *m_vertex_buffer; }

        const BufferGPU &index_buffer() const { return *m_index_buffer; }

        const BufferCPUtoGPU &uniform_buffer(uint32_t index) const { return *m_uniform_buffers[index]; }
        BufferCPUtoGPU &uniform_buffer(uint32_t index) { return *m_uniform_buffers[index]; }

        const UniformBufferDescriptorSet &uniform_buffer_descriptor_set() const
        {
            return *m_uniform_buffer_descriptor_set;
        }

        const ShaderModuleComponents &shader_modules() const { return *m_shader_modules; }

        const SwapchainComponents &swapchain() const { return *m_swapchain; }

        const RenderPassComponents &render_pass() const { return *m_render_pass; }

        const FramebufferComponents &framebuffers() const { return *m_framebuffers; }

        const GraphicsPipelineComponents &graphics_pipeline() const { return *m_graphics_pipeline; }

        friend class VulkanContextBuilder;
    };
}