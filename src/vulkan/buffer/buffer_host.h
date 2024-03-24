#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "buffer.h"

namespace levin
{
    class BufferHost: public Buffer
    {
    public:
        BufferHost(
            const Device &device,
            VkDeviceSize size,
            VkBufferUsageFlags usage):
            Buffer(
                device,
                size,
                usage,
                VMA_MEMORY_USAGE_AUTO,
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
                | VMA_ALLOCATION_CREATE_MAPPED_BIT)
        {
        }

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

}