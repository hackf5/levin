#include "buffer_transfer_queue.h"

#include "spdlog/spdlog.h"

using namespace levin;

BufferTransferQueue::BufferTransferQueue(
    DeviceComponents &device_components,
    size_t command_buffer_count):
    m_device_components(&device_components),
    m_command_factory(device_components.get_device()),
    m_queue(device_components.get_transfer_queue())
{
    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = m_command_factory
        .get_device()
        .get_queue_index(vkb::QueueType::transfer).value();
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    m_command_pool = m_command_factory.create_command_pool(pool_info);

    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = m_command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffer_count);

    m_command_buffers = m_command_factory.create_command_buffers(alloc_info);
}

VkCommandBuffer BufferTransferQueue::begin(size_t index) const
{
    assert(index < m_command_buffers.size());

    auto command_buffer = m_command_buffers[index];
    if (vkResetCommandBuffer(command_buffer, 0) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to reset transfer command buffer");
    }

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording transfer command buffer");
    }

    return command_buffer;
}

void BufferTransferQueue::submit_and_wait(size_t index) const
{
    assert(index < m_command_buffers.size());

    auto command_buffer = m_command_buffers[index];

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

