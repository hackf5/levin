#include "descriptor_pool.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorPool::DescriptorPool(const Device &device):
    m_device(device),
    m_descriptor_pool(create_descriptor_pool())
{
}

DescriptorPool::~DescriptorPool()
{
    spdlog::info("Destroying descriptor pool");
    vkDestroyDescriptorPool(m_device, m_descriptor_pool, nullptr);
}

VkDescriptorPool DescriptorPool::create_descriptor_pool()
{
    spdlog::info("Creating descriptor pool");

    VkDescriptorPoolSize uniform_buffer_size = {};
    uniform_buffer_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniform_buffer_size.descriptorCount = 1000;

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.poolSizeCount = 1;
    pool_info.pPoolSizes = &uniform_buffer_size;

    // this is the maximum number of descriptor sets that can be allocated from the pool
    // once it is necessary to allocate more sets than this a new pool must be created
    pool_info.maxSets = 1000;

    VkDescriptorPool descriptor_pool;
    if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &descriptor_pool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }

    return descriptor_pool;
}