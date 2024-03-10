#pragma once

#include "device_components.h"
#include "command_factory.h"

#include <vector>

namespace levin
{
    class BufferTransferQueue
    {
    private:
        DeviceComponents const * const m_device_components;

        CommandFactory m_command_factory;

        VkQueue m_queue;
        VkCommandPool m_command_pool;
        std::vector<VkCommandBuffer> m_command_buffers;

    public:
        BufferTransferQueue(
            DeviceComponents &device_components,
            size_t command_buffer_count = 1);
        BufferTransferQueue(const BufferTransferQueue &) = delete;

        VkCommandBuffer begin(size_t index = 0) const;

        void submit_and_wait(size_t index = 0) const;
    };
}