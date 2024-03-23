#include "descriptor_pool.h"

#include <array>
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
    // the number of descriptors of that type to allocate
    uniform_buffer_size.descriptorCount = 1000;

    VkDescriptorPoolSize sampler_size = {};
    sampler_size.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    sampler_size.descriptorCount = 1000;

    std::array<VkDescriptorPoolSize, 2> pool_sizes = { uniform_buffer_size, sampler_size };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    // the maximum number of descriptor sets that can be bound to the pipeline at once
    pool_info.maxSets = 4;

    pool_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
    pool_info.pPoolSizes = pool_sizes.data();

    VkDescriptorPool descriptor_pool;
    if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &descriptor_pool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }

    return descriptor_pool;
}