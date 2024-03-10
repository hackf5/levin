#pragma once

#include <memory>
#include <vector>

#include "device_components.h"
#include "descriptor_components.h"
#include "buffer.h"

namespace levin
{
    class DescriptorSetComponents
    {
    protected:
        DescriptorComponents const * const m_descriptor_components;
        DeviceComponents const * const m_device_components;

        std::vector<VkDescriptorSet> m_descriptor_sets;

        void create_descriptor_sets(size_t count);

    public:
        DescriptorSetComponents(
            const DeviceComponents &device_components,
            const DescriptorComponents &descriptor_components);
        DescriptorSetComponents(const DescriptorSetComponents &) = delete;

        ~DescriptorSetComponents();

        VkDescriptorSet get_descriptor_set(size_t index) const { return m_descriptor_sets[index]; }
    };

    class UniformBufferDescriptorSet: public DescriptorSetComponents
    {
    public:
        UniformBufferDescriptorSet(
            const DeviceComponents &device_components,
            const DescriptorComponents &descriptor_components,
            const std::vector<std::shared_ptr<BufferCPUtoGPU>> &uniform_buffers,
            size_t object_size);
    };
}