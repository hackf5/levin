#include "descriptor_factory.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorFactory::DescriptorFactory(const vkb::Device &device)
    : VulkanFactory(device)
{
}

VkDescriptorSetLayout DescriptorFactory::create_descriptor_set_layout(
    const VkDescriptorSetLayoutCreateInfo &create_info)
{
    spdlog::info("Creating Descriptor Set Layout");

    VkDescriptorSetLayout descriptor_set_layout;
    if (vkCreateDescriptorSetLayout(device(), &create_info, nullptr, &descriptor_set_layout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor set layout");
    }

    register_destruction([descriptor_set_layout](const vkb::Device &device)
        {
            spdlog::info("Destroying Descriptor Set Layout");
            vkDestroyDescriptorSetLayout(device, descriptor_set_layout, nullptr);
        });

    return descriptor_set_layout;
}

VkDescriptorPool DescriptorFactory::create_descriptor_pool(
    const VkDescriptorPoolCreateInfo &create_info)
{
    spdlog::info("Creating Descriptor Pool");

    VkDescriptorPool descriptor_pool;
    if (vkCreateDescriptorPool(device(), &create_info, nullptr, &descriptor_pool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }

    register_destruction([descriptor_pool](const vkb::Device &device)
        {
            spdlog::info("Destroying Descriptor Pool");
            vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
        });

    return descriptor_pool;
}