#pragma once

#include <memory>
#include <vector>

#include "window.h"
#include "device.h"
#include "buffer_transfer_queue.h"
#include "graphics_commands.h"
#include "descriptor_pool.h"
#include "descriptor_set_layout.h"
#include "buffer.h"
#include "vertex.h"
#include "model.h"
#include "descriptor_set.h"
#include "shader_module.h"
#include "swapchain.h"
#include "render_pass.h"
#include "framebuffers.h"
#include "graphics_pipeline.h"

namespace levin
{
    class VulkanContext
    {
    private:
        std::unique_ptr<Window> m_window;
        std::unique_ptr<Device> m_device;
        std::unique_ptr<BufferTransferQueue> m_transfer_queue;
        std::unique_ptr<GraphicsCommands> m_graphics_commands;
        std::unique_ptr<DescriptorPool> m_descriptor_pool;
        std::unique_ptr<DescriptorSetLayout> m_descriptor_set_layout;
        std::unique_ptr<Model> m_model;
        std::unique_ptr<ShaderModule> m_shader_modules;
        std::unique_ptr<Swapchain> m_swapchain;
        std::unique_ptr<RenderPass> m_render_pass;
        std::unique_ptr<Framebuffers> m_framebuffers;
        std::unique_ptr<GraphicsPipeline> m_graphics_pipeline;

    public:
        VulkanContext() = default;

        VulkanContext(const VulkanContext &) = delete;

        Window &window() { return *m_window; }

        const Device &device() const { return *m_device; }

        const BufferTransferQueue &transfer_queue() const { return *m_transfer_queue; }

        GraphicsCommands &graphics_commands() { return *m_graphics_commands; }
        const GraphicsCommands &graphics_commands() const { return *m_graphics_commands; }

        const DescriptorPool &descriptor_pool() const { return *m_descriptor_pool; }

        const DescriptorSetLayout &descriptor_set_layout() const { return *m_descriptor_set_layout; }

        const Model &model() const { return *m_model; }
        Model &model() { return *m_model; }

        const ShaderModule &shader_modules() const { return *m_shader_modules; }

        const Swapchain &swapchain() const { return *m_swapchain; }

        const RenderPass &render_pass() const { return *m_render_pass; }

        const Framebuffers &framebuffers() const { return *m_framebuffers; }

        const GraphicsPipeline &graphics_pipeline() const { return *m_graphics_pipeline; }

        friend class VulkanContextBuilder;
    };
}