#pragma once

#include <memory>
#include <vector>

#include "util/no_default_ctors.h"
#include "window.h"
#include "device.h"
#include "transfer_queue.h"
#include "graphics_queue.h"
#include "descriptor_pool.h"
#include "descriptor_set_layout.h"
#include "buffer.h"
#include "descriptor_set.h"
#include "shader_module.h"
#include "swapchain.h"
#include "render_pass.h"
#include "framebuffers.h"
#include "graphics_pipeline.h"

#include "model/vertex.h"
#include "model/model.h"
#include "model/camera.h"

#include "gui/gui.h"

namespace levin
{
    class VulkanContext : NoCopyOrMove
    {
    private:
        std::unique_ptr<Window> m_window;
        std::unique_ptr<Device> m_device;
        std::unique_ptr<TransferQueue> m_transfer_queue;
        std::unique_ptr<GraphicsQueue> m_graphics_queue;
        std::unique_ptr<DescriptorPool> m_descriptor_pool;
        std::unique_ptr<DescriptorSetLayout> m_descriptor_set_layout;
        std::unique_ptr<Model> m_model;
        std::unique_ptr<Camera> m_camera;
        std::unique_ptr<Swapchain> m_swapchain;
        std::unique_ptr<RenderPass> m_render_pass;
        std::unique_ptr<Framebuffers> m_framebuffers;
        std::unique_ptr<GraphicsPipeline> m_graphics_pipeline;
        std::unique_ptr<Gui> m_gui;

    public:
        VulkanContext() = default;

        Window &window() { return *m_window; }

        const Device &device() const { return *m_device; }

        const TransferQueue &transfer_queue() const { return *m_transfer_queue; }

        GraphicsQueue &graphics_queue() { return *m_graphics_queue; }
        const GraphicsQueue &graphics_queue() const { return *m_graphics_queue; }

        const DescriptorPool &descriptor_pool() const { return *m_descriptor_pool; }

        const DescriptorSetLayout &descriptor_set_layout() const { return *m_descriptor_set_layout; }

        const Model &model() const { return *m_model; }
        Model &model() { return *m_model; }

        const Camera &camera() const { return *m_camera; }
        Camera &camera() { return *m_camera; }

        const Swapchain &swapchain() const { return *m_swapchain; }

        const RenderPass &render_pass() const { return *m_render_pass; }

        const Framebuffers &framebuffers() const { return *m_framebuffers; }

        const GraphicsPipeline &graphics_pipeline() const { return *m_graphics_pipeline; }

        const Gui &gui() const { return *m_gui; }
        Gui &gui() { return *m_gui; }

        friend class VulkanContextBuilder;
    };
}