#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "util/no_copy_or_move.h"
#include "device.h"

namespace levin
{
    class TransferQueue : NoCopyOrMove
    {
    private:
        const Device &m_device;

        const VkQueue m_queue;
        const VkCommandPool m_command_pool;
        const VkCommandBuffer m_command_buffer;

        VkCommandPool create_command_pool();
        VkCommandBuffer create_command_buffer();

    public:
        TransferQueue(const Device &device);
        TransferQueue(const TransferQueue &) = delete;
        ~TransferQueue();

        VkCommandBuffer begin() const;

        void submit_and_wait() const;
    };
}