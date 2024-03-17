#pragma once

#include <vector>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "device.h"
#include "transfer_queue.h"

namespace levin
{
    class Buffer
    {
    protected:
        struct AllocationInfo
        {
            const VkBuffer buffer;
            const VmaAllocation allocation;
            const VmaAllocationInfo info;
        };

        const VmaAllocator m_allocator;

        const VkBufferUsageFlags m_usage;
        const VmaMemoryUsage m_memory_usage;
        const VmaAllocationCreateFlags m_allocation_flags;

        const AllocationInfo m_allocation_info;

        const VkDescriptorBufferInfo m_descriptor;

        AllocationInfo create_allocation_info(VkDeviceSize size);
        VkDescriptorBufferInfo create_descriptor_info() const;

    public:
        Buffer(
            const Device &device,
            VkDeviceSize size,
            VkBufferUsageFlags usage,
            VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_AUTO,
            VmaAllocationCreateFlags allocation_flags = 0);
        Buffer(const Buffer &) = delete;

        ~Buffer();

        VkDeviceSize size() const { return m_allocation_info.info.size; }

        operator VkBuffer() const { return m_allocation_info.buffer; }

        const VkDescriptorBufferInfo &descriptor() const { return m_descriptor; }
    };

    class BufferCPUtoGPU: public Buffer
    {
    public:
        BufferCPUtoGPU(
            const Device &device,
            VkDeviceSize size,
            VkBufferUsageFlags usage);
        BufferCPUtoGPU(const BufferCPUtoGPU &) = delete;

        void copy_from(void *data, VkDeviceSize size)
        {
            memcpy(m_allocation_info.info.pMappedData, data, size);
        }

        template <typename T>
        void copy_from(const std::vector<T> &data)
        {
            copy_from((void *)data.data(), sizeof(T) * data.size());
        }
    };

    class BufferGPU: public Buffer
    {
    private:
        const Device &m_device;
        const TransferQueue &m_transfer_queue;

    public:
        BufferGPU(
            const Device &device,
            const TransferQueue &transfer_queue,
            VkDeviceSize size,
            VkBufferUsageFlags usage);
        BufferGPU(const BufferGPU &) = delete;

        void copy_from(const BufferCPUtoGPU &buffer) const;

        template <typename T>
        void copy_from(const std::vector<T> &data) const
        {
            BufferCPUtoGPU staging_buffer(
                m_device,
                sizeof(T) * data.size(),
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            staging_buffer.copy_from(data);
            copy_from(staging_buffer);
        }
    };
}