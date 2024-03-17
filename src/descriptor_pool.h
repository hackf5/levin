#pragma once

#include <vulkan/vulkan.h>

#include "device.h"

namespace levin
{
    class DescriptorPool
    {
    private:
        const Device &m_device;
        const VkDescriptorPool m_descriptor_pool;

        VkDescriptorPool create_descriptor_pool();

    public:
        DescriptorPool(const Device &device);
        DescriptorPool(const DescriptorPool &) = delete;
        ~DescriptorPool();

        operator VkDescriptorPool() const { return m_descriptor_pool; }
    };
}