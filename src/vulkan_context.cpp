#include "vulkan_context.h"

#include <stdexcept>

using namespace levin;

VulkanContextBuilder &VulkanContextBuilder::configure_window(int width, int height, const std::string &title)
{
    m_context->m_window = std::make_unique<WindowComponents>(width, height, title);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_device(bool enableValidationLayers)
{
    if (!m_context->m_window)
    {
        throw std::runtime_error("Window must be configured before device");
    }

    m_context->m_device = std::make_unique<DeviceComponents>(*m_context->m_window, enableValidationLayers);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_uniform_buffers(VkDeviceSize size)
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before uniform buffers");
    }

    m_context->m_uniform_buffers.resize(VulkanContext::max_frames_in_flight);
    for (size_t i = 0; i < VulkanContext::max_frames_in_flight; i++)
    {
        m_context->m_uniform_buffers[i] = std::make_unique<BufferCPUtoGPU>(
            *m_context->m_device,
            size,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    }

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_descriptor_pool()
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before descriptor pool");
    }

    m_context->m_descriptor_components = std::make_unique<DescriptorComponents>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_uniform_buffer_descriptor_set(VkDeviceSize size)
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before uniform buffer descriptor set");
    }

    if (!m_context->m_descriptor_components)
    {
        throw std::runtime_error("Descriptor components must be configured before uniform buffer descriptor set");
    }

    if (m_context->m_uniform_buffers.empty())
    {
        throw std::runtime_error("Uniform buffers must be configured before uniform buffer descriptor set");
    }

    std::vector<VkBuffer> uniform_buffers;
    for (size_t i = 0; i < m_context->m_uniform_buffers.size(); i++)
    {
        uniform_buffers.push_back(*m_context->m_uniform_buffers[i]);
    }

    m_context->m_uniform_buffer_descriptor_set = std::make_unique<UniformBufferDescriptorSet>(
        *m_context->m_device,
        *m_context->m_descriptor_components,
        uniform_buffers.data(),
        uniform_buffers.size(),
        size);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_transfer_queue(size_t command_buffer_count)
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before transfer queue");
    }

    m_context->m_transfer_queue = std::make_unique<BufferTransferQueue>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_vertex_buffer(VkDeviceSize size)
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before vertex buffer");
    }

    if (!m_context->m_transfer_queue)
    {
        throw std::runtime_error("Transfer queue must be configured before vertex buffer");
    }

    m_context->m_vertex_buffer = std::make_unique<BufferGPU>(
        *m_context->m_device,
        *m_context->m_transfer_queue,
        size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_index_buffer(VkDeviceSize size)
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before index buffer");
    }

    if (!m_context->m_transfer_queue)
    {
        throw std::runtime_error("Transfer queue must be configured before index buffer");
    }

    m_context->m_index_buffer = std::make_unique<BufferGPU>(
        *m_context->m_device,
        *m_context->m_transfer_queue,
        size,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_graphics_commands()
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before graphics commands");
    }

    m_context->m_graphics_commands = std::make_unique<GraphicsCommands>(
        *m_context->m_device);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_swapchain()
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before swapchain");
    }

    m_context->m_swapchain = std::make_unique<SwapchainComponents>(*m_context->m_device);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_render_pass()
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before render pass");
    }

    if (!m_context->m_swapchain)
    {
        throw std::runtime_error("Swapchain must be configured before render pass");
    }

    m_context->m_render_pass = std::make_unique<RenderPassComponents>(
        *m_context->m_device,
        *m_context->m_swapchain);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_framebuffers()
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before framebuffers");
    }

    if (!m_context->m_swapchain)
    {
        throw std::runtime_error("Swapchain must be configured before framebuffers");
    }

    if (!m_context->m_render_pass)
    {
        throw std::runtime_error("Render pass must be configured before framebuffers");
    }

    if (m_context->m_framebuffers)
    {
        m_context->m_framebuffers.reset();
    }

    m_context->m_framebuffers = std::make_unique<FramebufferComponents>(
        *m_context->m_device,
        *m_context->m_swapchain,
        *m_context->m_render_pass);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_graphics_pipeline()
{
    if (!m_context->m_device)
    {
        throw std::runtime_error("Device must be configured before graphics pipeline");
    }

    if (!m_context->m_descriptor_components)
    {
        throw std::runtime_error("Descriptor components must be configured before graphics pipeline");
    }

    if (!m_context->m_swapchain)
    {
        throw std::runtime_error("Swapchain must be configured before graphics pipeline");
    }

    if (!m_context->m_render_pass)
    {
        throw std::runtime_error("Render pass must be configured before graphics pipeline");
    }

    m_context->m_graphics_pipeline = std::make_unique<GraphicsPipelineComponents>(
        *m_context->m_device,
        *m_context->m_descriptor_components,
        *m_context->m_swapchain,
        *m_context->m_render_pass);

    return *this;
}

std::unique_ptr<VulkanContext> VulkanContextBuilder::build()
{

    return std::move(m_context);
}