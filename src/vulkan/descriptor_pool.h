#pragma once

#include <vulkan/vulkan.h>

#include "util/no_copy_or_move.h"
#include "device.h"

namespace levin
{
    class DescriptorPool : NoCopyOrMove
    {
    private:
        const Device &m_device;
        const VkDescriptorPool m_descriptor_pool;

        VkDescriptorPool create_descriptor_pool();

    public:
        DescriptorPool(const Device &device);
        ~DescriptorPool();

        operator VkDescriptorPool() const { return m_descriptor_pool; }
    };
}