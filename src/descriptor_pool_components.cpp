#include "descriptor_pool_components.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorPoolComponents::DescriptorPoolComponents(const DeviceComponents &device):
    m_device(device),
    m_descriptor_pool(create_descriptor_pool())
{
}

DescriptorPoolComponents::~DescriptorPoolComponents()
{
    spdlog::info("Destroying descriptor pool");
    vkDestroyDescriptorPool(m_device, m_descriptor_pool, nullptr);
}

VkDescriptorPool DescriptorPoolComponents::create_descriptor_pool()
{
    // currently specific to uniform buffer
    spdlog::info("Creating descriptor pool");

    VkDescriptorPoolSize pool_size = {};
    pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size.descriptorCount = 100; // this is arbitrary and needs resolving

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.poolSizeCount = 1;
    pool_info.pPoolSizes = &pool_size;
    pool_info.maxSets = 100; // this is arbitrary and needs resolving

    VkDescriptorPool descriptor_pool;
    if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &descriptor_pool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }

    return descriptor_pool;
}