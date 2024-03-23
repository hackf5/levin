#pragma once

#include <array>
#include <memory>

#include <vulkan/vulkan.h>

#include "buffer/buffer_gpu.h"
#include "device.h"
#include "descriptor_pool.h"
#include "transfer_queue.h"
#include "vertex.h"

#include "util/no_copy_or_move.h"

namespace levin
{
    class GraphicsBuffers: NoCopyOrMove
    {
    private:
        const Device &m_device;
        const DescriptorPool &m_descriptor_pool;
        const TransferQueue &m_transfer_queue;

        std::unique_ptr<BufferGPU> m_vertex_buffer;
        std::unique_ptr<BufferGPU> m_index_buffer;

        std::array<VkBuffer, 1> m_vertex_buffers;

    public:
        GraphicsBuffers(
            const Device &device,
            const DescriptorPool &descriptor_pool,
            const TransferQueue &transfer_queue);

        void load_vertexes(const std::vector<levin::Vertex> &vertexes);
        void load_indexes(const std::vector<Vertex::index_t> &indexes);
        void bind(VkCommandBuffer command_buffer) const;
    };
}