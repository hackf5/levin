#include "buffer_transfer_queue.h"

#include "spdlog/spdlog.h"

using namespace levin;

BufferTransferQueue::BufferTransferQueue(const Device &device):
    m_factory(device),
    m_queue(device.transfer_queue()),
    m_command_pool(create_command_pool(device)),
    m_command_buffer(create_command_buffer())
{
}

VkCommandPool BufferTransferQueue::create_command_pool(const Device &device)
{
    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = device.transfer_queue_index();
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    return m_factory.create_command_pool(pool_info);
}

VkCommandBuffer BufferTransferQueue::create_command_buffer()
{
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = m_command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    return m_factory.create_command_buffers(alloc_info)[0];
}

VkCommandBuffer BufferTransferQueue::begin() const
{
    if (vkResetCommandBuffer(m_command_buffer, 0) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to reset transfer command buffer");
    }

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_command_buffer, &begin_info) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording transfer command buffer");
    }

    return m_command_buffer;
}

void BufferTransferQueue::submit_and_wait() const
{
    auto command_buffer = m_command_buffer;

    if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to record transfer command buffer");
    }

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffer;

    if (vkQueueSubmit(m_queue, 1, &submit_info, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit transfer command buffer");
    }

    if (vkQueueWaitIdle(m_queue) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to wait for queue to become idle");
    }
}

