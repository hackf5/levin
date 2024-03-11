#pragma once

#include <vulkan/vulkan.h>

#include "device_components.h"
#include "descriptor_pool_factory.h"

namespace levin
{
    class DescriptorPoolComponents
    {
    private:
        DescriptorPoolFactory m_factory;

        const VkDescriptorSetLayout m_descriptor_set_layout;
        const VkDescriptorPool m_descriptor_pool;

        VkDescriptorSetLayout create_descriptor_set_layout();
        VkDescriptorPool create_descriptor_pool();

    public:
        DescriptorPoolComponents(const DeviceComponents &device);
        DescriptorPoolComponents(const DescriptorPoolComponents &) = delete;

        const VkDescriptorSetLayout &layout() const { return m_descriptor_set_layout; }

        const VkDescriptorPool &pool() const { return m_descriptor_pool; }
    };
}