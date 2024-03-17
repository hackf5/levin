#include "model.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

Mesh::Mesh(
    const DeviceComponents &device,
    const DescriptorPoolComponents &descriptor_pool,
    const DescriptorSetLayout &descriptor_set_layout,
    std::vector<Primitive *> primitives):
    m_uniform_block {},
    m_primitives(primitives),
    m_uniform_buffers(create_uniform_buffers(device)),
    m_descriptor_sets(create_descriptor_sets(device, descriptor_pool, descriptor_set_layout))
{
}

std::vector<std::unique_ptr<BufferCPUtoGPU>> Mesh::create_uniform_buffers(const DeviceComponents &device)
{
    std::vector<std::unique_ptr<BufferCPUtoGPU>> uniform_buffers;

    uniform_buffers.reserve(DeviceComponents::max_frames_in_flight);
    for (size_t i = 0; i < DeviceComponents::max_frames_in_flight; i++)
    {
        auto buffer = std::make_unique<BufferCPUtoGPU>(
            device,
            sizeof(m_uniform_block),
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

        uniform_buffers.push_back(std::move(buffer));
    }
    return uniform_buffers;
}

std::vector<std::unique_ptr<UniformBufferDescriptorSet>> Mesh::create_descriptor_sets(
    const DeviceComponents &device,
    const DescriptorPoolComponents &descriptor_pool,
    const DescriptorSetLayout &descriptor_set_layout)
{
    std::vector<std::unique_ptr<UniformBufferDescriptorSet>> descriptor_sets;

    for (const auto &buffer : m_uniform_buffers)
    {
        auto set = std::make_unique<UniformBufferDescriptorSet>(
            device,
            descriptor_pool,
            descriptor_set_layout,
            *buffer);

        descriptor_sets.push_back(std::move(set));
    }

    return descriptor_sets;
}

Model::Model(
    const DeviceComponents &device,
    const DescriptorPoolComponents &descriptor_pool,
    const BufferTransferQueue &transfer_queue):
    m_device(device),
    m_descriptor_pool(descriptor_pool),
    m_transfer_queue(transfer_queue)
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
    m_vertex_count = vertexes.size();
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
    m_index_count = indexes.size();
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