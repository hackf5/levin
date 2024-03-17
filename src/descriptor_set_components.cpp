#include "descriptor_set_components.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorSetComponents::DescriptorSetComponents(
    const DeviceComponents &device,
    const DescriptorPoolComponents &descriptor_pool,
    const DescriptorSetLayout &descriptor_set_layout):
    m_device(device),
    m_descriptor_pool(descriptor_pool),
    m_descriptor_set_layout(descriptor_set_layout),
    m_descriptor_sets(create_descriptor_sets())
{
}

DescriptorSetComponents::~DescriptorSetComponents()
{
    vkFreeDescriptorSets(
        m_device,
        m_descriptor_pool,
        static_cast<uint32_t>(m_descriptor_sets.size()),
        m_descriptor_sets.data());
}

std::vector<VkDescriptorSet> DescriptorSetComponents::create_descriptor_sets()
{
    std::vector<VkDescriptorSetLayout> layouts(
        DeviceComponents::max_frames_in_flight,
        m_descriptor_set_layout);

    VkDescriptorSetAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = m_descriptor_pool;
    alloc_info.descriptorSetCount = DeviceComponents::max_frames_in_flight;
    alloc_info.pSetLayouts = layouts.data();

    std::vector<VkDescriptorSet> descriptor_sets(DeviceComponents::max_frames_in_flight);
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
    const DescriptorSetLayout &descriptor_set_layout,
    std::vector<Buffer*> &uniform_buffers):
    DescriptorSetComponents(
    device,
    descriptor_pool,
    descriptor_set_layout)
{
    for (size_t i = 0; i < m_descriptor_sets.size(); i++)
    {
        auto buffer_info = uniform_buffers[i]->descriptor_info();

        VkWriteDescriptorSet descriptor_write {};
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