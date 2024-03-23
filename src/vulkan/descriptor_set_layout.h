#pragma once

#include <vulkan/vulkan.h>

#include "util/no_copy_or_move.h"
#include "device.h"

namespace levin
{
    class DescriptorSetLayout : NoCopyOrMove
    {
    private:
        const Device &m_device;
        const VkDescriptorSetLayout m_descriptor_set_layout;

        VkDescriptorSetLayout create_descriptor_set_layout();

    public:
        DescriptorSetLayout(const Device &device);
        ~DescriptorSetLayout();

        operator VkDescriptorSetLayout () const { return m_descriptor_set_layout; }
    };
}