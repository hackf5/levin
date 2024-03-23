#include "graphics_buffers.h"

using namespace levin;

const VkDeviceSize vertex_offsets[] = { 0 };

GraphicsBuffers::GraphicsBuffers(
    const Device &device,
    const DescriptorPool &descriptor_pool,
    const TransferQueue &transfer_queue):
    m_device(device),
    m_descriptor_pool(descriptor_pool),
    m_transfer_queue(transfer_queue),
    m_vertex_buffer(nullptr),
    m_index_buffer(nullptr),
    m_vertex_buffers()
{
}

void GraphicsBuffers::load_vertexes(const std::vector<Vertex> &vertexes)
{
    m_vertex_buffer.reset();

    m_vertex_buffer = std::make_unique<BufferGPU>(
        m_device,
        m_transfer_queue,
        sizeof(vertexes[0]) * vertexes.size(),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    m_vertex_buffers[0] = *m_vertex_buffer;

    m_vertex_buffer->copy_from(vertexes);
}

void GraphicsBuffers::load_indexes(const std::vector<Vertex::index_t> &indexes)
{
    m_index_buffer.reset();

    m_index_buffer = std::make_unique<BufferGPU>(
        m_device,
        m_transfer_queue,
        sizeof(indexes[0]) * indexes.size(),
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    m_index_buffer->copy_from(indexes);
}

void GraphicsBuffers::bind(VkCommandBuffer command_buffer) const
{
    vkCmdBindVertexBuffers(command_buffer, 0, 1, m_vertex_buffers.data(), vertex_offsets);
    vkCmdBindIndexBuffer(command_buffer, *m_index_buffer, 0, Vertex::vk_index_type);
}