#pragma once

#include "buffer.h"
#include "device_components.h"
#include "command_factory.h"
#include "vertex.h"

namespace levin
{
    class BufferComponents
    {
    private:
        std::shared_ptr<DeviceComponents> m_device_components;

        CommandFactory m_command_factory;

        VkQueue m_transfer_queue;
        VkCommandPool m_transfer_command_pool;
        VkCommandBuffer m_transfer_command_buffer;

        Buffer m_vertex_buffer;
        Buffer m_index_buffer;

        static VkCommandPool create_command_pool(CommandFactory &command_factory);

        static VkCommandBuffer create_command_buffer(
            CommandFactory &command_factory,
            VkCommandPool command_pool);

        void load_buffer(
            void *data,
            Buffer &buffer,
            VkDeviceSize size);

    public:
        BufferComponents(const std::shared_ptr<DeviceComponents> &device_components);

        Buffer &get_vertex_buffer() { return m_vertex_buffer; }
        Buffer &get_index_buffer() { return m_index_buffer; }

        void load_vertex_buffer(const std::vector<Vertex> &vertices);
        void load_index_buffer(const std::vector<Vertex::index_t> &indices);
    };
}