#pragma once

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class DescriptorPoolFactory: public VulkanFactory
    {
    public:
        DescriptorPoolFactory(const vkb::Device &device);
        DescriptorPoolFactory(const DescriptorPoolFactory &) = delete;

        VkDescriptorSetLayout create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo &create_info);

        VkDescriptorPool create_descriptor_pool(const VkDescriptorPoolCreateInfo &create_info);
    };
}