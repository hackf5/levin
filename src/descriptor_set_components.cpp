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
    m_descriptor_set(create_descriptor_set(descriptor_set_layout))
{
}

DescriptorSetComponents::~DescriptorSetComponents()
{
    vkFreeDescriptorSets(
        m_device,
        m_descriptor_pool,
        1,
        &m_descriptor_set);
}

VkDescriptorSet DescriptorSetComponents::create_descriptor_set(
    const DescriptorSetLayout &descriptor_set_layout)
{
    std::vector<VkDescriptorSetLayout> layouts = { descriptor_set_layout };

    VkDescriptorSetAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = m_descriptor_pool;
    alloc_info.descriptorSetCount = 1;
    alloc_info.pSetLayouts = layouts.data();

    VkDescriptorSet descriptor_set;
    if (vkAllocateDescriptorSets(m_device, &alloc_info, &descriptor_set) != VK_SUCCESS)
    {
        spdlog::error("Failed to allocate descriptor set");
        throw std::runtime_error("Failed to allocate descriptor set");
    }

    return descriptor_set;
}

UniformBufferDescriptorSet::UniformBufferDescriptorSet(
    const DeviceComponents &device,
    const DescriptorPoolComponents &descriptor_pool,
    const DescriptorSetLayout &descriptor_set_layout,
    const Buffer &uniform_buffer):
    DescriptorSetComponents(
    device,
    descriptor_pool,
    descriptor_set_layout)
{
    auto buffer_info = uniform_buffer.descriptor_info();

    VkWriteDescriptorSet descriptor_write {};
    descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet = m_descriptor_set;
    descriptor_write.dstBinding = 0;
    descriptor_write.dstArrayElement = 0;
    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_write.descriptorCount = 1;
    descriptor_write.pBufferInfo = &buffer_info;

    vkUpdateDescriptorSets(m_device, 1, &descriptor_write, 0, nullptr);
}