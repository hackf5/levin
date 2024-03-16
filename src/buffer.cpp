#include "buffer.h"

#include "spdlog/spdlog.h"

using namespace levin;

Buffer::Buffer(
    const DeviceComponents &device,
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VmaMemoryUsage memory_usage,
    VmaAllocationCreateFlags allocation_flags):
    m_allocator(device.allocator()),
    m_usage(usage),
    m_memory_usage(memory_usage),
    m_allocation_flags(allocation_flags),
    m_allocation_info(create_allocation_info(size))
{
}

Buffer::~Buffer()
{
    vmaDestroyBuffer(m_allocator, *this, *this);
}

Buffer::AllocationInfo Buffer::create_allocation_info(VkDeviceSize size)
{
    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = m_usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo alloc_info = {};
    alloc_info.usage = m_memory_usage;
    alloc_info.flags = m_allocation_flags;

    VkBuffer buffer;
    VmaAllocation allocation;
    VmaAllocationInfo allocation_info;
    if (vmaCreateBuffer(m_allocator, &buffer_info, &alloc_info, &buffer, &allocation, &allocation_info) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer");
    }

    return {buffer, allocation, allocation_info};
}

BufferCPUtoGPU::BufferCPUtoGPU(
    const DeviceComponents &device,
    VkDeviceSize size,
    VkBufferUsageFlags usage):
    Buffer(
    device,
    size,
    usage,
    VMA_MEMORY_USAGE_AUTO,
    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT)
{
}

BufferGPU::BufferGPU(
    const DeviceComponents &device,
    const BufferTransferQueue &transfer_queue,
    VkDeviceSize size,
    VkBufferUsageFlags usage):
    Buffer(
    device,
    size,
    usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT),
    m_device(device),
    m_transfer_queue(transfer_queue)
{
}

void BufferGPU::copy_from(const BufferCPUtoGPU &buffer) const
{
    auto command_buffer = m_transfer_queue.begin();

    VkBufferCopy copy_region = {};
    copy_region.size = buffer.size();
    vkCmdCopyBuffer(command_buffer, buffer, *this, 1, &copy_region);

    m_transfer_queue.submit_and_wait();
}