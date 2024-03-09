#pragma once

#include <vma/vk_mem_alloc.h>

#include "device_components.h"

namespace levin
{
    class Buffer
    {
    private:
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

        VkBuffer get_buffer() { return m_buffer; }

        void copy_data(void *data, VkDeviceSize size);

        operator VkBuffer() { return m_buffer; }
    };
}