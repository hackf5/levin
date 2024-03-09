#include "vulkan_engine.h"

#include "uniform_buffer_object.h"
#include "vertex.h"

#include "spdlog/spdlog.h"

using namespace levin;

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

VulkanEngine::VulkanEngine(
    const std::shared_ptr<WindowComponents> &window_components,
    const std::shared_ptr<DeviceComponents> &device_components):
    m_window_components(window_components),
    m_device_components(device_components),
    m_transfer_queue(std::make_shared<BufferTransferQueue>(device_components)),
    m_render_pass(RenderPassComponents(device_components)),
    m_swapchain(std::make_unique<SwapchainComponents>(device_components, m_render_pass.get_render_pass())),
    m_graphics_pipeline(GraphicsPipelineComponents(device_components, m_render_pass.get_render_pass())),
    m_graphics_commands(GraphicsCommands(device_components))
{
    m_vertex_buffer = std::make_unique<BufferGPU>(
        device_components,
        m_transfer_queue,
        sizeof(vertices[0]) * vertices.size(),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    m_index_buffer = std::make_unique<BufferGPU>(
        device_components,
        m_transfer_queue,
        sizeof(indices[0]) * indices.size(),
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    m_uniform_buffer = std::make_unique<BufferCPUtoGPU>(
        device_components,
        sizeof(UniformBufferObject),
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

void VulkanEngine::run()
{
    spdlog::info("Vulkan Engine is running");

    load_vertexes();
    load_indexes();

    while (!m_window_components->should_close())
    {
        m_window_components->poll_events();
        draw_frame();
    }

    m_device_components->wait_idle();
}

void VulkanEngine::draw_frame()
{
    auto result = m_graphics_commands.acquire_next_image(m_current_frame, m_swapchain->get_swapchain(), m_image_index);

    if (result == GraphicsResult::RecreateSwapchain)
    {
        recreate_swapchain();
        return;
    }

    m_graphics_commands.reset_command_buffer(m_current_frame);

    record_command_buffer();

    m_graphics_commands.submit_command_buffer(m_current_frame);

    result = m_graphics_commands.present(m_current_frame, m_swapchain->get_swapchain(), m_image_index);
    if (result == GraphicsResult::RecreateSwapchain)
    {
        recreate_swapchain();
    }

    next_frame();
}

void VulkanEngine::recreate_swapchain()
{
    m_window_components->wait_resize();
    m_device_components->wait_idle();
    m_swapchain.reset();
    m_swapchain = std::make_unique<SwapchainComponents>(m_device_components, m_render_pass.get_render_pass());
}

void VulkanEngine::record_command_buffer()
{
    auto command_buffer = m_graphics_commands.begin_command_buffer(m_current_frame);
    auto extent = m_swapchain->get_extent();

    VkRenderPassBeginInfo render_pass_info {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = m_render_pass.get_render_pass();
    render_pass_info.framebuffer = m_swapchain->get_framebuffer(m_image_index);
    render_pass_info.renderArea.offset = { 0, 0 };
    render_pass_info.renderArea.extent = extent;

    VkClearValue clear_color = { 0.0f, 0.0f, 0.0f, 1.0f };
    render_pass_info.clearValueCount = 1;
    render_pass_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(
        command_buffer,
        &render_pass_info,
        VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(
        command_buffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_graphics_pipeline.get_pipeline());

    VkViewport viewport {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor {};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    VkBuffer vertex_buffers[] = { *m_vertex_buffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(command_buffer, *m_index_buffer, 0, Vertex::vk_index_type);
    vkCmdDrawIndexed(command_buffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(command_buffer);

    m_graphics_commands.end_command_buffer(m_current_frame);
}

void VulkanEngine::load_vertexes()
{
    auto staging_buffer = BufferCPUtoGPU(
        m_device_components,
        sizeof(vertices[0]) * vertices.size(),
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    staging_buffer.copy_from((void *)vertices.data(), sizeof(vertices[0]) * vertices.size());
    m_vertex_buffer->copy_from(staging_buffer);
}

void VulkanEngine::load_indexes()
{
    auto staging_buffer = BufferCPUtoGPU(
        m_device_components,
        sizeof(indices[0]) * indices.size(),
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    staging_buffer.copy_from((void *)indices.data(), sizeof(indices[0]) * indices.size());
    m_index_buffer->copy_from(staging_buffer);
}
