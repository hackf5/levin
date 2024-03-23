#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "buffer/buffer.h"
#include "device.h"
#include "descriptor_pool.h"
#include "descriptor_set_layout.h"
#include "graphics_pipeline.h"

namespace levin
{
    class DescriptorSet : NoCopyOrMove
    {
    private:
        VkDescriptorSet create_descriptor_set(const DescriptorSetLayout &descriptor_set_layout);

    protected:
        const Device &m_device;
        const DescriptorPool &m_descriptor_pool;
        const VkDescriptorSet m_descriptor_set;

    public:
        DescriptorSet(
            const Device &device,
            const DescriptorPool &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout);
        ~DescriptorSet();

        operator VkDescriptorSet() const { return m_descriptor_set; }
    };
}