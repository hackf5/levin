#pragma once

#include <vulkan/vulkan.h>

#include "device.h"

namespace levin
{
    class DescriptorPoolComponents
    {
    private:
        const Device &m_device;
        const VkDescriptorPool m_descriptor_pool;

        VkDescriptorPool create_descriptor_pool();

    public:
        DescriptorPoolComponents(const Device &device);
        DescriptorPoolComponents(const DescriptorPoolComponents &) = delete;
        ~DescriptorPoolComponents();

        operator VkDescriptorPool() const { return m_descriptor_pool; }
    };
}