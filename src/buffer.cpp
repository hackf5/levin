#include "buffer.h"

#include "spdlog/spdlog.h"

using namespace levin;

Buffer::Buffer(
    const DeviceComponents &device_components,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VmaMemoryUsage memory_usage,
    VmaAllocationCreateFlags flags):
    m_allocator(device_components.get_allocator()),
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

BufferCPUtoGPU::BufferCPUtoGPU(
    const DeviceComponents &device_components,
    VkDeviceSize size,
    VkBufferUsageFlags usage):
    Buffer(
    device_components,
    size,
    usage,
    VMA_MEMORY_USAGE_AUTO,
    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT)
{
    if (vmaMapMemory(m_allocator, m_allocation, &m_mapped_data) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to map CPU to GPU buffer memory");
    }
}

BufferCPUtoGPU::~BufferCPUtoGPU()
{
    vmaUnmapMemory(m_allocator, m_allocation);
}

void BufferCPUtoGPU::copy_from(void *data, VkDeviceSize size)
{
    memcpy(m_mapped_data, data, size);
}

BufferGPU::BufferGPU(
    const DeviceComponents &device_components,
    const BufferTransferQueue &transfer_queue,
    VkDeviceSize size,
    VkBufferUsageFlags usage):
    Buffer(
    device_components,
    size,
    usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT),
    m_transfer_queue(&transfer_queue)
{
}

void BufferGPU::copy_from(const BufferCPUtoGPU &buffer) const
{
    auto command_buffer = m_transfer_queue->begin();

    VkBufferCopy copy_region = {};
    copy_region.size = buffer.size();
    vkCmdCopyBuffer(command_buffer, buffer, m_buffer, 1, &copy_region);

    m_transfer_queue->submit_and_wait();
}