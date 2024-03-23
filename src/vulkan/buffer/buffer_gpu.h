#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "buffer.h"
#include "buffer_host.h"

namespace levin
{
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
            VkBufferUsageFlags usage):
            Buffer(
            device,
            size,
            usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT),
            m_device(device),
            m_transfer_queue(transfer_queue)
        {
        };

        void copy_from(const BufferHost &buffer) const
        {
            auto command_buffer = m_transfer_queue.begin();

            VkBufferCopy copy_region = {};
            copy_region.size = buffer.size();
            vkCmdCopyBuffer(command_buffer, buffer, *this, 1, &copy_region);

            m_transfer_queue.submit_and_wait();
        }

        template <typename T>
        void copy_from(const std::vector<T> &data) const
        {
            BufferHost staging_buffer(
                m_device,
                sizeof(T) * data.size(),
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            staging_buffer.copy_from(data);
            copy_from(staging_buffer);
        }
    };
}