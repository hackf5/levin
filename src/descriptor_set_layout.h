#pragma once

#include <vulkan/vulkan.h>

#include "device_components.h"

namespace levin
{
    class DescriptorSetLayout
    {
    private:
        const DeviceComponents &m_device;
        const VkDescriptorSetLayout m_descriptor_set_layout;

        VkDescriptorSetLayout create_descriptor_set_layout();

    public:
        DescriptorSetLayout(const DeviceComponents &device);
        DescriptorSetLayout(const DescriptorSetLayout &) = delete;
        ~DescriptorSetLayout();

        operator VkDescriptorSetLayout () const { return m_descriptor_set_layout; }
    };
}