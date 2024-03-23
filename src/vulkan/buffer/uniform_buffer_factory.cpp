#include "uniform_buffer_factory.h"

using namespace levin;

UniformBufferFactory::UniformBufferFactory(
    const Device &device,
    const DescriptorSetLayout &descriptor_set_layout):
    m_device(device),
    m_descriptor_set_layout(descriptor_set_layout),
    m_descriptor_pools()
{
    m_descriptor_pools.emplace_back(std::make_unique<DescriptorPool>(m_device));
}

std::unique_ptr<UniformBuffer> UniformBufferFactory::create(VkDeviceSize size, UniformBuffer::Usage usage)
{
    try
    {
        return create_internal(size, usage);
    }
    catch (const std::runtime_error &e)
    {
        // If we failed to create a descriptor set, create a new descriptor pool and try again
        m_descriptor_pools.emplace_back(std::make_unique<DescriptorPool>(m_device));
        return create_internal(size, usage);
    }
}

std::unique_ptr<UniformBuffer> UniformBufferFactory::create_internal(VkDeviceSize size, UniformBuffer::Usage usage) const
{
    return std::make_unique<UniformBuffer>(
        m_device,
        m_descriptor_set_layout,
        *m_descriptor_pools.back(),
        size,
        usage);
}