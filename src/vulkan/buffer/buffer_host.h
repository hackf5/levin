#pragma once

#include <type_traits>
#include <vector>

#include <vulkan/vulkan.h>

#include "buffer.h"

namespace levin
{
class BufferHost: public Buffer
{
private:
    void copy_from(void *data, VkDeviceSize size)
    {
        memcpy(m_allocation_info.info.pMappedData, data, size);
    }

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

    template <typename T>
    void copy_from(const T &data)
    {
        copy_from((void *)&data, sizeof(data));
    }

    template <typename TIter>
    void copy_from(TIter begin, TIter end)
    {
        static_assert(std::contiguous_iterator<TIter>, "TIter must be a contiguous iterator");

        copy_from((void *)&*begin, sizeof(*begin) * std::distance(begin, end));
    }
};
}