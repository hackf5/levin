#pragma once

#include <array>
#include <memory>
#include <type_traits>

#include <vulkan/vulkan.h>

#include "buffer/buffer_gpu.h"
#include "device.h"
#include "adhoc_queues.h"
#include "vertex.h"

#include "util/no_copy_or_move.h"

namespace levin
{
class GraphicsBuffers: NoCopyOrMove
{
private:
    const Device &m_device;
    const AdhocQueues &m_adhoc_queues;

    std::unique_ptr<BufferGPU> m_vertex_buffer;
    std::unique_ptr<BufferGPU> m_index_buffer;

    std::array<VkBuffer, 1> m_vertex_buffers;

public:
    GraphicsBuffers(
        const Device &device,
        const AdhocQueues &adhoc_queues);

    template <typename TIter>
    void load_vertexes(TIter begin, TIter end)
    {
        static_assert(
            std::is_same_v<Vertex, std::decay_t<decltype(*begin)>>,
            "TIter must be an iterator to Vertex");

        static_assert(std::contiguous_iterator<TIter>, "TIter must be a contiguous iterator");

        m_vertex_buffer.reset();
        m_vertex_buffer = std::make_unique<BufferGPU>(
            m_device,
            m_adhoc_queues,
            sizeof(*begin) * std::distance(begin, end),
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        m_vertex_buffer->copy_from(begin, end);

        m_vertex_buffers[0] = *m_vertex_buffer;
    }

    template <typename TIter>
    void load_indexes(TIter begin, TIter end)
    {
        static_assert(
            std::is_same_v<Vertex::index_t, std::decay_t<decltype(*begin)>>,
            "TIter must be an iterator to Vertex::index_t");

        static_assert(std::contiguous_iterator<TIter>, "TIter must be a contiguous iterator");

        m_index_buffer.reset();
        m_index_buffer = std::make_unique<BufferGPU>(
            m_device,
            m_adhoc_queues,
            sizeof(*begin) * std::distance(begin, end),
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        m_index_buffer->copy_from(begin, end);
    }

    void bind(VkCommandBuffer command_buffer) const;
};
}