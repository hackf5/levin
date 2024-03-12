#include "graphics_commands.h"

#include <limits>

#include <spdlog/spdlog.h>

using namespace levin;

GraphicsCommands::GraphicsCommands(const DeviceComponents &device_components):
    m_device_components(device_components),
    m_command_factory(device_components),
    m_graphics_queue(device_components.graphics_queue()),
    m_command_pool(create_command_pool()),
    m_command_buffers(create_command_buffers()),
    m_image_available(m_command_factory.create_semaphores(DeviceComponents::max_frames_in_flight)),
    m_render_finished(m_command_factory.create_semaphores(DeviceComponents::max_frames_in_flight)),
    m_in_flight_fences(m_command_factory.create_fences(DeviceComponents::max_frames_in_flight))
{
}

VkCommandPool GraphicsCommands::create_command_pool()
{
    VkCommandPoolCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.queueFamilyIndex = m_device_components.graphics_queue_index();
    create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    return m_command_factory.create_command_pool(create_info);
}

std::vector<VkCommandBuffer> GraphicsCommands::create_command_buffers()
{
    VkCommandBufferAllocateInfo allocate_info {};
    allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.commandPool = m_command_pool;
    allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandBufferCount = DeviceComponents::max_frames_in_flight;

    return m_command_factory.create_command_buffers(allocate_info);
}

VkFramebuffer GraphicsCommands::prepare_framebuffer(
    uint32_t current_frame,
    VkSwapchainKHR swapchain,
    const FramebufferComponents &framebuffers)
{
    assert(m_swapchain == VK_NULL_HANDLE);

    m_current_frame = current_frame;
    m_swapchain = swapchain;

    vkWaitForFences(
        m_device_components,
        1,
        &m_in_flight_fences[m_current_frame],
        VK_TRUE,
        std::numeric_limits<uint64_t>::max());

    vkResetFences(m_device_components, 1, &m_in_flight_fences[m_current_frame]);

    VkResult result = vkAcquireNextImageKHR(
        m_device_components,
        swapchain,
        std::numeric_limits<uint64_t>::max(),
        m_image_available[m_current_frame],
        VK_NULL_HANDLE,
        &m_image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return nullptr;
    }
    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    return framebuffers.framebuffer(m_image_index);
}

VkCommandBuffer GraphicsCommands::begin_command() const
{
    assert(m_swapchain != VK_NULL_HANDLE);

    if (vkResetCommandBuffer(m_command_buffers[m_current_frame], 0) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to reset command buffer");
    }

    VkCommandBufferBeginInfo begin_info {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_command_buffers[m_current_frame], &begin_info) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer");
    }

    return m_command_buffers[m_current_frame];
}

void GraphicsCommands::submit_command() const
{
    assert(m_swapchain != VK_NULL_HANDLE);

    if (vkEndCommandBuffer(m_command_buffers[m_current_frame]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to end recording command buffer");
    }

    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_command_buffers[m_current_frame];

    VkSemaphore wait_semaphores[] = { m_image_available[m_current_frame] };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;

    VkSemaphore signal_semaphores[] = { m_render_finished[m_current_frame] };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    vkResetFences(m_device_components, 1, &m_in_flight_fences[m_current_frame]);

    if (vkQueueSubmit(
        m_graphics_queue,
        1,
        &submit_info,
        m_in_flight_fences[m_current_frame]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit draw command buffer");
    }
}

bool GraphicsCommands::present_framebuffer()
{
    assert(m_swapchain != VK_NULL_HANDLE);
    auto swapchain = m_swapchain;
    m_swapchain = VK_NULL_HANDLE;

    VkPresentInfoKHR present_info {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    VkSemaphore wait_semaphores[] = { m_render_finished[m_current_frame] };
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = wait_semaphores;

    VkSwapchainKHR swapchains[] = { swapchain };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapchains;
    present_info.pImageIndices = &m_image_index;

    VkResult result = vkQueuePresentKHR(m_graphics_queue, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        return false;
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swap chain image");
    }

    return true;
}