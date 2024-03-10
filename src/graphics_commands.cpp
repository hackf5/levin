#include "graphics_commands.h"

#include <limits>

#include <spdlog/spdlog.h>

using namespace levin;

GraphicsCommands::GraphicsCommands(const DeviceComponents &device_components):
    m_graphics_queue(device_components.get_graphics_queue()),
    m_command_factory(device_components),
    m_command_pool(create_command_pool(m_command_factory)),
    m_command_buffers(create_command_buffers(m_command_factory, m_command_pool)),
    m_image_available(m_command_factory.create_semaphores(DeviceComponents::frames_in_flight)),
    m_render_finished(m_command_factory.create_semaphores(DeviceComponents::frames_in_flight)),
    m_in_flight_fences(m_command_factory.create_fences(DeviceComponents::frames_in_flight))
{
}

VkCommandPool GraphicsCommands::create_command_pool(CommandFactory &command_factory)
{
    VkCommandPoolCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    create_info.queueFamilyIndex = command_factory
        .device()
        .get_queue_index(vkb::QueueType::graphics).value();
    create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    return command_factory.create_command_pool(create_info);
}

std::vector<VkCommandBuffer> GraphicsCommands::create_command_buffers(
    CommandFactory &command_factory,
    VkCommandPool command_pool)
{
    VkCommandBufferAllocateInfo allocate_info {};
    allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.commandPool = command_pool;
    allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandBufferCount = DeviceComponents::frames_in_flight;

    return command_factory.create_command_buffers(allocate_info);
}

void GraphicsCommands::reset_command_buffer(uint32_t index) const
{
    vkResetCommandBuffer(m_command_buffers[index], 0);
}

VkCommandBuffer GraphicsCommands::begin_command_buffer(uint32_t index) const
{
    VkCommandBufferBeginInfo begin_info {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_command_buffers[index], &begin_info) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer");
    }

    return m_command_buffers[index];
}

void GraphicsCommands::end_command_buffer(uint32_t index) const
{
    if (vkEndCommandBuffer(m_command_buffers[index]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record command buffer");
    }
}

void GraphicsCommands::submit_command_buffer(uint32_t index) const
{
    VkSubmitInfo submit_info {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_command_buffers[index];

    VkSemaphore wait_semaphores[] = { m_image_available[index] };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;

    VkSemaphore signal_semaphores[] = { m_render_finished[index] };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    vkResetFences(m_command_factory.device(), 1, &m_in_flight_fences[index]);

    if (vkQueueSubmit(
        m_graphics_queue,
        1,
        &submit_info,
        m_in_flight_fences[index]) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit draw command buffer");
    }
}

GraphicsResult GraphicsCommands::acquire_next_image(
    uint32_t index,
    VkSwapchainKHR swapchain)
{
    vkWaitForFences(
        m_command_factory.device(),
        1,
        &m_in_flight_fences[index],
        VK_TRUE,
        std::numeric_limits<uint64_t>::max());

    vkResetFences(m_command_factory.device(), 1, &m_in_flight_fences[index]);

    VkResult result = vkAcquireNextImageKHR(
        m_command_factory.device(),
        swapchain,
        std::numeric_limits<uint64_t>::max(),
        m_image_available[index],
        VK_NULL_HANDLE,
        &m_image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return GraphicsResult::RecreateSwapchain;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    return GraphicsResult::Success;
}

GraphicsResult GraphicsCommands::present(
    uint32_t index,
    VkSwapchainKHR swapchain) const
{
    VkPresentInfoKHR present_info {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    VkSemaphore wait_semaphores[] = { m_render_finished[index] };
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = wait_semaphores;

    VkSwapchainKHR swapchains[] = { swapchain };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapchains;
    present_info.pImageIndices = &m_image_index;

    VkResult result = vkQueuePresentKHR(m_graphics_queue, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        return GraphicsResult::RecreateSwapchain;
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swap chain image");
    }

    return GraphicsResult::Success;
}