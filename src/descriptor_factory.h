#pragma once

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class DescriptorFactory: public VulkanFactory
    {
    public:
        DescriptorFactory(vkb::Device &device);

        VkDescriptorSetLayout create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo &create_info);

        VkDescriptorPool create_descriptor_pool(const VkDescriptorPoolCreateInfo &create_info);
    };
}