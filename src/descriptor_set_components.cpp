#include "descriptor_set_components.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorSetComponents::DescriptorSetComponents(
    const DeviceComponents &device,
    const DescriptorPoolComponents &descriptor_pool,
    size_t count):
    m_device(device),
    m_descriptor_pool(descriptor_pool),
    m_descriptor_sets(create_descriptor_sets(count))
{
}

DescriptorSetComponents::~DescriptorSetComponents()
{
    vkFreeDescriptorSets(
        m_device,
        m_descriptor_pool.pool(),
        static_cast<uint32_t>(m_descriptor_sets.size()),
        m_descriptor_sets.data());
}

std::vector<VkDescriptorSet> DescriptorSetComponents::create_descriptor_sets(size_t count)
{
    std::vector<VkDescriptorSetLayout> layouts(count, m_descriptor_pool.layout());

    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = m_descriptor_pool.pool();
    alloc_info.descriptorSetCount = static_cast<uint32_t>(count);
    alloc_info.pSetLayouts = layouts.data();

    std::vector<VkDescriptorSet> descriptor_sets(count);
    if (vkAllocateDescriptorSets(m_device, &alloc_info, descriptor_sets.data()) != VK_SUCCESS)
    {
        spdlog::error("Failed to allocate descriptor sets");
        throw std::runtime_error("Failed to allocate descriptor sets");
    }

    return descriptor_sets;
}

UniformBufferDescriptorSet::UniformBufferDescriptorSet(
    const DeviceComponents &device,
    const DescriptorPoolComponents &descriptor_pool,
    VkBuffer* uniform_buffers,
    size_t count,
    size_t object_size):
    DescriptorSetComponents(device, descriptor_pool, count)
{
    for (size_t i = 0; i < count; i++)
    {
        VkDescriptorBufferInfo buffer_info{};
        buffer_info.buffer = uniform_buffers[i];
        buffer_info.offset = 0;
        buffer_info.range = object_size;

        VkWriteDescriptorSet descriptor_write{};
        descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptor_write.dstSet = m_descriptor_sets[i];
        descriptor_write.dstBinding = 0;
        descriptor_write.dstArrayElement = 0;
        descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptor_write.descriptorCount = 1;
        descriptor_write.pBufferInfo = &buffer_info;

        vkUpdateDescriptorSets(m_device, 1, &descriptor_write, 0, nullptr);
    }
}