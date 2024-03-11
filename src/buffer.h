#pragma once

#include <vma/vk_mem_alloc.h>

#include "device_components.h"
#include "buffer_transfer_queue.h"

namespace levin
{
    class Buffer
    {
    protected:
        struct AllocationInfo
        {
            const VkBuffer buffer;
            const VmaAllocation allocation;
            const VmaAllocationInfo allocation_info;
        };

        const VmaAllocator m_allocator;

        const VkDeviceSize m_size;
        const VkBufferUsageFlags m_usage;
        const VmaMemoryUsage m_memory_usage;
        const VmaAllocationCreateFlags m_allocation_flags;

        const AllocationInfo m_allocation_info;

        AllocationInfo create_allocation_info();

    public:
        Buffer(
            const DeviceComponents &device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_AUTO,
            VmaAllocationCreateFlags allocation_flags = 0);
        Buffer(const Buffer &) = delete;

        ~Buffer();

        VkDeviceSize size() const { return m_size; }

        operator VkBuffer() const { return m_allocation_info.buffer; }

        operator VmaAllocation() const { return m_allocation_info.allocation; }

        operator VmaAllocationInfo() const { return m_allocation_info.allocation_info; }
    };

    class BufferCPUtoGPU: public Buffer
    {
    private:
        void *m_mapped_data;

    public:
        BufferCPUtoGPU(
            const DeviceComponents &device,
            VkDeviceSize size,
            VkBufferUsageFlags usage);
        BufferCPUtoGPU(const BufferCPUtoGPU &) = delete;

        ~BufferCPUtoGPU();

        void copy_from(void *data, VkDeviceSize size);
    };

    class BufferGPU: public Buffer
    {
        const BufferTransferQueue& m_transfer_queue;

    public:
        BufferGPU(
            const DeviceComponents &device,
            const BufferTransferQueue &transfer_queue,
            VkDeviceSize size,
            VkBufferUsageFlags usage);
        BufferGPU(const BufferGPU &) = delete;

        void copy_from(const BufferCPUtoGPU &buffer) const;
    };
}