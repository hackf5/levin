#include "buffer_components.h"

#include "spdlog/spdlog.h"

using namespace levin;

BufferComponents::BufferComponents(const std::shared_ptr<DeviceComponents> &device_components):
    m_device_components(device_components),
    m_command_factory(device_components->get_device()),
    m_transfer_queue(device_components->get_transfer_queue()),
    m_transfer_command_pool(create_command_pool(m_command_factory)),
    m_transfer_command_buffer(create_command_buffer(m_command_factory, m_transfer_command_pool)),
    m_vertex_buffer(
    device_components,
    100 * sizeof(Vertex),
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT),
    m_index_buffer(
    device_components,
    100 * sizeof(uint32_t),
    VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
{
}

VkCommandPool BufferComponents::create_command_pool(CommandFactory &command_factory)
{
    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.queueFamilyIndex = command_factory
        .get_device()
        .get_queue_index(vkb::QueueType::transfer).value();
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    return command_factory.create_command_pool(pool_info);
}

VkCommandBuffer BufferComponents::create_command_buffer(
    CommandFactory &command_factory,
    VkCommandPool command_pool)
{
    VkCommandBufferAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.commandPool = command_pool;
    alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info.commandBufferCount = 1;

    return command_factory.create_command_buffers(alloc_info)[0];
}

void BufferComponents::load_vertex_buffer(const std::vector<Vertex> &vertices)
{
    VkDeviceSize buffer_size = sizeof(vertices[0]) * vertices.size();
    load_buffer((void*)vertices.data(), m_vertex_buffer, buffer_size);
}

void BufferComponents::load_index_buffer(const std::vector<Vertex::index_t> &indices)
{
    VkDeviceSize buffer_size = sizeof(indices[0]) * indices.size();
    load_buffer((void*)indices.data(), m_index_buffer, buffer_size);
}

void BufferComponents::load_buffer(
    void *data,
    Buffer &buffer,
    VkDeviceSize size)
{
    Buffer staging_buffer(
        m_device_components,
        size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VMA_MEMORY_USAGE_AUTO,
        VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT |
        VMA_ALLOCATION_CREATE_MAPPED_BIT);

    staging_buffer.copy_data(data, size);

    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkResetCommandBuffer(m_transfer_command_buffer, 0);
    vkBeginCommandBuffer(m_transfer_command_buffer, &begin_info);

    VkBufferCopy copy_region = {};
    copy_region.size = size;
    vkCmdCopyBuffer(
        m_transfer_command_buffer,
        staging_buffer.get_buffer(),
        buffer.get_buffer(),
        1,
        &copy_region);

    vkEndCommandBuffer(m_transfer_command_buffer);

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_transfer_command_buffer;

    vkQueueSubmit(m_transfer_queue, 1, &submit_info, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_transfer_queue);
}