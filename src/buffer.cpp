#include "buffer.h"

#include "spdlog/spdlog.h"

using namespace levin;

Buffer::Buffer(
    const std::shared_ptr<DeviceComponents> &device_components,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VmaMemoryUsage memory_usage,
    VmaAllocationCreateFlags flags):
    m_allocator(device_components->get_allocator()),
    m_size(size),
    m_usage(usage),
    m_memory_usage(memory_usage),
    m_flags(flags)
{
    create_buffer();
}

Buffer::~Buffer()
{
    spdlog::info("Destroying Buffer");
    vmaDestroyBuffer(m_allocator, m_buffer, m_allocation);
}

void Buffer::create_buffer()
{
    spdlog::info("Creating Buffer");

    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = m_size;
    buffer_info.usage = m_usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = m_memory_usage;
    alloc_info.flags = m_flags;

    if (vmaCreateBuffer(m_allocator, &buffer_info, &alloc_info, &m_buffer, &m_allocation, &m_allocation_info) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer");
    }
}

void Buffer::copy_data(void *data, VkDeviceSize size)
{
    spdlog::info("Copying Data to Buffer");

    void *mapped_data;
    if (vmaMapMemory(m_allocator, m_allocation, &mapped_data) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to map memory");
    }

    memcpy(mapped_data, data, size);

    vmaUnmapMemory(m_allocator, m_allocation);
}