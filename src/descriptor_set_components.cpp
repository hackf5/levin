#include "descriptor_set_components.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorSetComponents::DescriptorSetComponents(
    const DeviceComponents &device_components,
    const DescriptorComponents &descriptor_components):
    m_device_components(&device_components),
    m_descriptor_components(&descriptor_components)
{
}

DescriptorSetComponents::~DescriptorSetComponents()
{
    vkFreeDescriptorSets(
        *m_device_components,
        m_descriptor_components->pool(),
        static_cast<uint32_t>(m_descriptor_sets.size()),
        m_descriptor_sets.data());
}

void DescriptorSetComponents::create_descriptor_sets(size_t count)
{
    if (m_descriptor_sets.size() > 0)
    {
        throw std::runtime_error("Descriptor sets already created");
    }

    std::vector<VkDescriptorSetLayout> layouts(count, m_descriptor_components->layout());

    VkDescriptorSetAllocateInfo alloc_info{};
    alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    alloc_info.descriptorPool = m_descriptor_components->pool();
    alloc_info.descriptorSetCount = static_cast<uint32_t>(count);
    alloc_info.pSetLayouts = layouts.data();

    m_descriptor_sets.resize(count);
    if (vkAllocateDescriptorSets(*m_device_components, &alloc_info, m_descriptor_sets.data()) != VK_SUCCESS)
    {
        spdlog::error("Failed to allocate descriptor sets");
        throw std::runtime_error("Failed to allocate descriptor sets");
    }
}

UniformBufferDescriptorSet::UniformBufferDescriptorSet(
    const DeviceComponents &device_components,
    const DescriptorComponents &descriptor_components,
    const std::vector<std::shared_ptr<BufferCPUtoGPU>> &uniform_buffers,
    size_t object_size):
    DescriptorSetComponents(device_components, descriptor_components)
{
    create_descriptor_sets(uniform_buffers.size());

    for (size_t i = 0; i < uniform_buffers.size(); i++)
    {
        VkDescriptorBufferInfo buffer_info{};
        buffer_info.buffer = *uniform_buffers[i];
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

        vkUpdateDescriptorSets(*m_device_components, 1, &descriptor_write, 0, nullptr);
    }
}