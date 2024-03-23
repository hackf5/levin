#include "vulkan_context_builder.h"

#include <stdexcept>

using namespace levin;

VulkanContextBuilder &VulkanContextBuilder::add_window(int width, int height, const std::string &title)
{
    m_context->m_window = std::make_unique<Window>(width, height, title);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_device(bool enableValidationLayers)
{
    m_context->m_device = std::make_unique<Device>(*m_context->m_window, enableValidationLayers);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_adhoc_queues()
{
    m_context->m_adhoc_queues = std::make_unique<AdhocQueues>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_graphics_queue()
{
    m_context->m_graphics_queue = std::make_unique<GraphicsQueue>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_descriptor_set_layout()
{
    m_context->m_descriptor_set_layout = std::make_unique<DescriptorSetLayout>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_graphics_buffers()
{
    m_context->m_graphics_buffers = std::make_unique<GraphicsBuffers>(
        *m_context->m_device,
        *m_context->m_adhoc_queues);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_uniform_buffer_factory()
{
    m_context->m_uniform_buffer_factory = std::make_unique<UniformBufferFactory>(
        *m_context->m_device,
        *m_context->m_descriptor_set_layout);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_sampler()
{
    m_context->m_sampler = std::make_unique<Sampler>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_scene()
{
    m_context->m_scene = std::make_unique<Scene>(*m_context->m_uniform_buffer_factory);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_swapchain()
{
    if (m_context->m_swapchain.get() != nullptr)
    {
        m_context->m_gui.reset();
        m_context->m_graphics_pipeline.reset();
        m_context->m_framebuffers.reset();
        m_context->m_render_pass.reset();
        m_context->m_swapchain.reset();
    }

    m_context->m_swapchain = std::make_unique<Swapchain>(*m_context->m_device);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_render_pass()
{
    m_context->m_render_pass = std::make_unique<RenderPass>(
        *m_context->m_device,
        *m_context->m_swapchain);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_framebuffers()
{
    m_context->m_framebuffers = std::make_unique<Framebuffers>(
        *m_context->m_device,
        *m_context->m_swapchain,
        *m_context->m_render_pass);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_graphics_pipeline()
{
    m_context->m_graphics_pipeline = std::make_unique<GraphicsPipeline>(
        *m_context->m_device,
        *m_context->m_descriptor_set_layout,
        *m_context->m_swapchain,
        *m_context->m_render_pass);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::add_gui()
{
    m_context->m_gui = std::make_unique<Gui>(
        *m_context->m_window,
        *m_context->m_device,
        *m_context->m_render_pass);

    return *this;
}

std::unique_ptr<VulkanContext> VulkanContextBuilder::build()
{
    return std::move(m_context);
}