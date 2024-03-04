#include "vulkan_engine.h"

#include "spdlog/spdlog.h"

using namespace levin;

VulkanEngine::VulkanEngine(
    const std::shared_ptr<WindowComponents> &window_components,
    const std::shared_ptr<DeviceComponents> &device_components):
    m_window_components(window_components),
    m_device_components(device_components),
    m_render_pass(RenderPassComponents(device_components)),
    m_swapchain(std::make_unique<SwapchainComponents>(device_components, m_render_pass.get_render_pass())),
    m_graphics_pipeline(GraphicsPipelineComponents(device_components, m_render_pass.get_render_pass())),
    m_graphics_commands(GraphicsCommands(device_components))
{
    spdlog::info("Vulkan Engine is starting");
}

void VulkanEngine::run()
{
    spdlog::info("Vulkan Engine is running");

    while (!m_window_components->should_close())
    {
        m_window_components->poll_events();
        draw_frame();
    }

    m_device_components->wait_idle();
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
    m_graphics_commands.reset_command_buffer(m_current_frame);
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

    vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline.get_pipeline());

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

    vkCmdDraw(command_buffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(command_buffer);

    m_graphics_commands.end_command_buffer(m_current_frame);
}

void VulkanEngine::draw_frame()
{
    auto result = m_graphics_commands.acquire_next_image(m_current_frame, m_swapchain->get_swapchain(), m_image_index);

    if (result == GraphicsResult::RecreateSwapchain)
    {
        recreate_swapchain();
        return;
    }

    record_command_buffer();

    m_graphics_commands.submit_command_buffer(m_current_frame);

    result = m_graphics_commands.present(m_current_frame, m_swapchain->get_swapchain(), m_image_index);
    if (result == GraphicsResult::RecreateSwapchain)
    {
        recreate_swapchain();
    }

    next_frame();
}