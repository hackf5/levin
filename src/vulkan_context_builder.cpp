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

VulkanContextBuilder &VulkanContextBuilder::configure_descriptor_set_layout()
{
    m_context->m_descriptor_set_layout = std::make_unique<DescriptorSetLayout>(*m_context->m_device);
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
    m_context->m_uniform_buffer_descriptor_sets.resize(DeviceComponents::max_frames_in_flight);
    for (size_t i = 0; i < m_context->m_uniform_buffers.size(); i++)
    {
        m_context->m_uniform_buffer_descriptor_sets[i] =
            std::make_unique<UniformBufferDescriptorSet>(
                *m_context->m_device,
                *m_context->m_descriptor_pool,
                *m_context->m_descriptor_set_layout,
                *m_context->m_uniform_buffers[i]);
    }

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
        *m_context->m_descriptor_set_layout,
        *m_context->m_shader_modules,
        *m_context->m_swapchain,
        *m_context->m_render_pass);

    return *this;
}

std::unique_ptr<VulkanContext> VulkanContextBuilder::build()
{
    return std::move(m_context);
}