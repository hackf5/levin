#include "descriptor_factory.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorFactory::DescriptorFactory(const vkb::Device &device)
    : VulkanFactory(device)
{
}

VkDescriptorSetLayout DescriptorFactory::create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo &create_info)
{
    spdlog::info("Creating Descriptor Set Layout");

    VkDescriptorSetLayout descriptor_set_layout;
    if (vkCreateDescriptorSetLayout(get_device(), &create_info, nullptr, &descriptor_set_layout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor set layout");
    }

    register_destruction([=, this]()
        {
            spdlog::info("Destroying Descriptor Set Layout");
            vkDestroyDescriptorSetLayout(get_device(), descriptor_set_layout, nullptr);
        });

    return descriptor_set_layout;
}

VkDescriptorPool DescriptorFactory::create_descriptor_pool(const VkDescriptorPoolCreateInfo &create_info)
{
    spdlog::info("Creating Descriptor Pool");

    VkDescriptorPool descriptor_pool;
    if (vkCreateDescriptorPool(get_device(), &create_info, nullptr, &descriptor_pool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }

    register_destruction([=, this]()
        {
            spdlog::info("Destroying Descriptor Pool");
            vkDestroyDescriptorPool(get_device(), descriptor_pool, nullptr);
        });

    return descriptor_pool;
}