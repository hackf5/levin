#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "device.h"
#include "command_factory.h"


namespace levin
{
    class BufferTransferQueue
    {
    private:
        CommandFactory m_factory;

        const VkQueue m_queue;
        const VkCommandPool m_command_pool;
        const VkCommandBuffer m_command_buffer;

        VkCommandPool create_command_pool(const Device &device);
        VkCommandBuffer create_command_buffer();

    public:
        BufferTransferQueue(const Device &device);
        BufferTransferQueue(const BufferTransferQueue &) = delete;

        VkCommandBuffer begin() const;

        void submit_and_wait() const;
    };
}