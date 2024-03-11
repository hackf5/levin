#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "device_components.h"
#include "descriptor_pool_components.h"

namespace levin
{
    class DescriptorSetComponents
    {
    private:
        std::vector<VkDescriptorSet> create_descriptor_sets(size_t count);

    protected:
        const DeviceComponents &m_device;
        const DescriptorPoolComponents &m_descriptor_pool;
        const std::vector<VkDescriptorSet> m_descriptor_sets;

    public:
        DescriptorSetComponents(
            const DeviceComponents &device,
            const DescriptorPoolComponents &descriptor_pool,
            size_t count);
        DescriptorSetComponents(const DescriptorSetComponents &) = delete;

        ~DescriptorSetComponents();

        VkDescriptorSet descriptor_set(size_t index) const { return m_descriptor_sets[index]; }
    };

    class UniformBufferDescriptorSet: public DescriptorSetComponents
    {
    public:
        UniformBufferDescriptorSet(
            const DeviceComponents &device,
            const DescriptorPoolComponents &descriptor_pool,
            VkBuffer *uniform_buffers,
            size_t count,
            size_t object_size);
    };
}