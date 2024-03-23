#include "descriptor_set.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorSet::DescriptorSet(
    const Device &device,
    const DescriptorPool &descriptor_pool,
    const DescriptorSetLayout &descriptor_set_layout):
    m_device(device),
    m_descriptor_pool(descriptor_pool),
    m_descriptor_set(create_descriptor_set(descriptor_set_layout))
{
}

DescriptorSet::~DescriptorSet()
{
    vkFreeDescriptorSets(
        m_device,
        m_descriptor_pool,
        1,
        &m_descriptor_set);
}

VkDescriptorSet DescriptorSet::create_descriptor_set(
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