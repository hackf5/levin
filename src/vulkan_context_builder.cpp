#include "vulkan_context_builder.h"

#include <stdexcept>

using namespace levin;

VulkanContextBuilder &VulkanContextBuilder::configure_window(int width, int height, const std::string &title)
{
    m_context->m_window = std::make_unique<Window>(width, height, title);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_device(bool enableValidationLayers)
{
    m_context->m_device = std::make_unique<Device>(*m_context->m_window, enableValidationLayers);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_transfer_queue()
{
    m_context->m_transfer_queue = std::make_unique<TransferQueue>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_graphics_queue()
{
    m_context->m_graphics_queue = std::make_unique<GraphicsQueue>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_descriptor_pool()
{
    m_context->m_descriptor_pool = std::make_unique<DescriptorPool>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_descriptor_set_layout()
{
    m_context->m_descriptor_set_layout = std::make_unique<DescriptorSetLayout>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_model()
{
    m_context->m_model = std::make_unique<Model>(
        *m_context->m_device,
        *m_context->m_descriptor_pool,
        *m_context->m_transfer_queue);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_camera()
{
    m_context->m_camera = std::make_unique<Camera>(
        *m_context->m_device,
        *m_context->m_descriptor_pool,
        *m_context->m_descriptor_set_layout);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_swapchain()
{
    if (m_context->m_swapchain.get() != nullptr)
    {
        m_context->m_graphics_pipeline.reset();
        m_context->m_framebuffers.reset();
        m_context->m_render_pass.reset();
        m_context->m_swapchain.reset();
    }

    m_context->m_swapchain = std::make_unique<Swapchain>(*m_context->m_device);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_render_pass()
{
    m_context->m_render_pass = std::make_unique<RenderPass>(
        *m_context->m_device,
        *m_context->m_swapchain);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_framebuffers()
{
    m_context->m_framebuffers = std::make_unique<Framebuffers>(
        *m_context->m_device,
        *m_context->m_swapchain,
        *m_context->m_render_pass);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_graphics_pipeline()
{
    m_context->m_graphics_pipeline = std::make_unique<GraphicsPipeline>(
        *m_context->m_device,
        *m_context->m_descriptor_set_layout,
        *m_context->m_swapchain,
        *m_context->m_render_pass);

    return *this;
}

std::unique_ptr<VulkanContext> VulkanContextBuilder::build()
{
    return std::move(m_context);
}