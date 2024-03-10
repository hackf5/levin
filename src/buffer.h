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
            const DeviceComponents &device_components,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_AUTO,
            VmaAllocationCreateFlags flags = 0);
        Buffer(const Buffer &) = delete;

        ~Buffer();

        VkDeviceSize size() const { return m_size; }

        operator VkBuffer() const { return m_buffer; }
    };

    class BufferCPUtoGPU: public Buffer
    {
    private:
        void *m_mapped_data;

    public:
        BufferCPUtoGPU(
            const DeviceComponents &device_components,
            VkDeviceSize size,
            VkBufferUsageFlags usage);
        BufferCPUtoGPU(const BufferCPUtoGPU &) = delete;

        ~BufferCPUtoGPU();

        void copy_from(void *data, VkDeviceSize size);
    };

    class BufferGPU: public Buffer
    {
        BufferTransferQueue const * const m_transfer_queue;

    public:
        BufferGPU(
            const DeviceComponents &device_components,
            const BufferTransferQueue &transfer_queue,
            VkDeviceSize size,
            VkBufferUsageFlags usage);
        BufferGPU(const BufferGPU &) = delete;

        void copy_from(const BufferCPUtoGPU &buffer) const;
    };
}