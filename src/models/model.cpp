#include "model.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

Mesh::Mesh(
    const Device &device,
    const DescriptorPoolComponents &descriptor_pool,
    const DescriptorSetLayout &descriptor_set_layout,
    std::vector<Primitive *> primitives):
    m_uniform_block {},
    m_primitives(primitives),
    m_uniform_buffer(create_uniform_buffer(device)),
    m_descriptor_set(create_descriptor_set(device, descriptor_pool, descriptor_set_layout))
{
}

std::unique_ptr<BufferCPUtoGPU> Mesh::create_uniform_buffer(const Device &device)
{
    auto buffer = std::make_unique<BufferCPUtoGPU>(
        device,
        sizeof(m_uniform_block),
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

    return std::move(buffer);
}

std::unique_ptr<UniformBufferDescriptorSet> Mesh::create_descriptor_set(
    const Device &device,
    const DescriptorPoolComponents &descriptor_pool,
    const DescriptorSetLayout &descriptor_set_layout)
{

    auto set = std::make_unique<UniformBufferDescriptorSet>(
        device,
        descriptor_pool,
        descriptor_set_layout,
        *m_uniform_buffer);

    return std::move(set);
}

Model::Model(
    const Device &device,
    const DescriptorPoolComponents &descriptor_pool,
    const BufferTransferQueue &transfer_queue):
    m_device(device),
    m_descriptor_pool(descriptor_pool),
    m_transfer_queue(transfer_queue),
    m_vertex_buffer(nullptr),
    m_index_buffer(nullptr),
    m_primitives(),
    m_root_node(std::make_unique<Node>(nullptr, nullptr))
{
}

void Model::load_vertexes(const std::vector<levin::Vertex> &vertexes)
{
    m_vertex_buffer.reset();

    m_vertex_buffer = std::make_unique<BufferGPU>(
        m_device,
        m_transfer_queue,
        sizeof(vertexes[0]) * vertexes.size(),
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

    m_vertex_buffer->copy_from(vertexes);
}

void Model::load_indexes(const std::vector<uint32_t> &indexes)
{
    m_index_buffer.reset();

    m_index_buffer = std::make_unique<BufferGPU>(
        m_device,
        m_transfer_queue,
        sizeof(indexes[0]) * indexes.size(),
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

    m_index_buffer->copy_from(indexes);
}

void Model::load_primitives(std::vector<std::unique_ptr<Primitive>> &primitives)
{
    m_primitives.clear();
    m_primitives.reserve(primitives.size());
    for (int i = 0; i < primitives.size(); i++)
    {
        m_primitives.push_back(std::move(primitives[i]));
    }
}