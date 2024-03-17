#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "device.h"

namespace levin
{
    class BufferTransferQueue
    {
    private:
        const Device &m_device;

        const VkQueue m_queue;
        const VkCommandPool m_command_pool;
        const VkCommandBuffer m_command_buffer;

        VkCommandPool create_command_pool();
        VkCommandBuffer create_command_buffer();

    public:
        BufferTransferQueue(const Device &device);
        BufferTransferQueue(const BufferTransferQueue &) = delete;
        ~BufferTransferQueue();

        VkCommandBuffer begin() const;

        void submit_and_wait() const;
    };
}