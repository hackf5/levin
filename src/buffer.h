#pragma once

#include <vma/vk_mem_alloc.h>

#include "device_components.h"
#include "buffer_transfer_queue.h"

namespace levin
{
    class Buffer
    {
    protected:
        VmaAllocator m_allocator;

        VkDeviceSize m_size;
        VkBufferUsageFlags m_usage;
        VmaMemoryUsage m_memory_usage;
        VmaAllocationCreateFlags m_flags;

        VkBuffer m_buffer;
        VmaAllocation m_allocation;
        VmaAllocationInfo m_allocation_info;

        void create_buffer();
    public:
        Buffer(
            const std::shared_ptr<DeviceComponents> &device_components,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_AUTO,
            VmaAllocationCreateFlags flags = 0);
        ~Buffer();

        VkDeviceSize size() { return m_size; }

        operator VkBuffer() { return m_buffer; }
    };

    class BufferCPUtoGPU: public Buffer
    {
    private:
        void *m_mapped_data;

    public:
        BufferCPUtoGPU(
            const std::shared_ptr<DeviceComponents> &device_components,
            VkDeviceSize size,
            VkBufferUsageFlags usage);
        ~BufferCPUtoGPU();

        void copy_from(void *data, VkDeviceSize size);
    };

    class BufferGPU: public Buffer
    {
        std::shared_ptr<BufferTransferQueue> m_transfer_queue;

    public:
        BufferGPU(
            const std::shared_ptr<DeviceComponents> &device_components,
            const std::shared_ptr<BufferTransferQueue> &transfer_queue,
            VkDeviceSize size,
            VkBufferUsageFlags usage);

        void copy_from(BufferCPUtoGPU &buffer);
    };
}