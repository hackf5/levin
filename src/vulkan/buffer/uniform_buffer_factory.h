#pragma once

#include <vector>
#include <memory>

#include <vulkan/vulkan.h>

#include "vulkan/device.h"
#include "vulkan/descriptor_pool.h"
#include "vulkan/descriptor_set_layout.h"

#include "uniform_buffer.h"

namespace levin
{
    class UniformBufferFactory
    {
    private:
        const Device &m_device;
        const DescriptorSetLayout &m_descriptor_set_layout;

        std::vector<std::unique_ptr<DescriptorPool>> m_descriptor_pools;

        std::unique_ptr<UniformBuffer> create_internal(VkDeviceSize size, UniformBuffer::Usage usage) const;

    public:
        UniformBufferFactory(
            const Device &device,
            const DescriptorSetLayout &descriptor_set_layout);

        std::unique_ptr<UniformBuffer> create(VkDeviceSize size, UniformBuffer::Usage usage);
    };
}
