#pragma once

#include <vulkan/vulkan.h>

#include "device_components.h"

namespace levin
{
    class DescriptorPoolComponents
    {
    private:
        const DeviceComponents &m_device;
        const VkDescriptorPool m_descriptor_pool;

        VkDescriptorPool create_descriptor_pool();

    public:
        DescriptorPoolComponents(const DeviceComponents &device);
        DescriptorPoolComponents(const DescriptorPoolComponents &) = delete;
        ~DescriptorPoolComponents();

        operator VkDescriptorPool() const { return m_descriptor_pool; }
    };
}