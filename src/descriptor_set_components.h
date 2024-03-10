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
    private:
        std::vector<VkDescriptorSet> create_descriptor_sets(size_t count);

    protected:
        DeviceComponents const *const m_device_components;

        DescriptorComponents const *const m_descriptor_components;

        std::vector<VkDescriptorSet> m_descriptor_sets;

    public:
        DescriptorSetComponents(
            const DeviceComponents &device_components,
            const DescriptorComponents &descriptor_components,
            size_t count);
        DescriptorSetComponents(const DescriptorSetComponents &) = delete;

        ~DescriptorSetComponents();

        VkDescriptorSet descriptor_set(size_t index) const { return m_descriptor_sets[index]; }
    };

    class UniformBufferDescriptorSet: public DescriptorSetComponents
    {
    public:
        UniformBufferDescriptorSet(
            const DeviceComponents &device_components,
            const DescriptorComponents &descriptor_components,
            VkBuffer *uniform_buffers,
            size_t count,
            size_t object_size);
    };
}