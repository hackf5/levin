#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "device_components.h"
#include "command_factory.h"


namespace levin
{
    class BufferTransferQueue
    {
    private:
        const DeviceComponents &m_device;

        CommandFactory m_factory;

        const VkQueue m_queue;
        const VkCommandPool m_command_pool;
        const std::vector<VkCommandBuffer> m_command_buffers;

        VkCommandPool create_command_pool();
        std::vector<VkCommandBuffer> create_command_buffers(size_t count);

    public:
        BufferTransferQueue(
            const DeviceComponents &device,
            size_t command_buffer_count = 1);
        BufferTransferQueue(const BufferTransferQueue &) = delete;

        VkCommandBuffer begin(size_t index = 0) const;

        void submit_and_wait(size_t index = 0) const;
    };
}