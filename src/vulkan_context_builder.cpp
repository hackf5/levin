#include "vulkan_context_builder.h"

#include <stdexcept>

using namespace levin;

VulkanContextBuilder &VulkanContextBuilder::configure_window(int width, int height, const std::string &title)
{
    m_context->m_window = std::make_unique<WindowComponents>(width, height, title);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_device(bool enableValidationLayers)
{
    m_context->m_device = std::make_unique<DeviceComponents>(*m_context->m_window, enableValidationLayers);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_transfer_queue(size_t command_buffer_count)
{
    m_context->m_transfer_queue = std::make_unique<BufferTransferQueue>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_graphics_commands()
{
    m_context->m_graphics_commands = std::make_unique<GraphicsCommands>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_descriptor_pool()
{
    m_context->m_descriptor_pool = std::make_unique<DescriptorPoolComponents>(*m_context->m_device);
    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_uniform_buffers(VkDeviceSize size)
{
    m_context->m_uniform_buffers.resize(DeviceComponents::max_frames_in_flight);
    for (size_t i = 0; i < DeviceComponents::max_frames_in_flight; i++)
    {
        m_context->m_uniform_buffers[i] = std::make_unique<BufferCPUtoGPU>(
            *m_context->m_device,
            size,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    }

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_uniform_buffer_descriptor_set(VkDeviceSize size)
{
    std::vector<VkBuffer> uniform_buffers;
    for (size_t i = 0; i < m_context->m_uniform_buffers.size(); i++)
    {
        uniform_buffers.push_back(*m_context->m_uniform_buffers[i]);
    }

    m_context->m_uniform_buffer_descriptor_set = std::make_unique<UniformBufferDescriptorSet>(
        *m_context->m_device,
        *m_context->m_descriptor_pool,
        uniform_buffers.data(),
        size);

    return *this;
}


VulkanContextBuilder &VulkanContextBuilder::configure_vertex_buffer(VkDeviceSize size)
{
    m_context->m_vertex_buffer = std::make_unique<BufferGPU>(
        *m_context->m_device,
        *m_context->m_transfer_queue,
        size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_index_buffer(VkDeviceSize size)
{
    m_context->m_index_buffer = std::make_unique<BufferGPU>(
        *m_context->m_device,
        *m_context->m_transfer_queue,
        size,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_shader_modules()
{
    m_context->m_shader_modules = std::make_unique<ShaderModuleComponents>(*m_context->m_device);
    m_context->m_shader_modules->load("vert");
    m_context->m_shader_modules->load("frag");
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

    m_context->m_swapchain = std::make_unique<SwapchainComponents>(*m_context->m_device);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_render_pass()
{
    m_context->m_render_pass = std::make_unique<RenderPassComponents>(
        *m_context->m_device,
        *m_context->m_swapchain);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_framebuffers()
{
    m_context->m_framebuffers = std::make_unique<FramebufferComponents>(
        *m_context->m_device,
        *m_context->m_swapchain,
        *m_context->m_render_pass);

    return *this;
}

VulkanContextBuilder &VulkanContextBuilder::configure_graphics_pipeline()
{
    m_context->m_graphics_pipeline = std::make_unique<GraphicsPipelineComponents>(
        *m_context->m_device,
        *m_context->m_descriptor_pool,
        *m_context->m_shader_modules,
        *m_context->m_swapchain,
        *m_context->m_render_pass);

    return *this;
}

std::unique_ptr<VulkanContext> VulkanContextBuilder::build()
{
    return std::move(m_context);
}