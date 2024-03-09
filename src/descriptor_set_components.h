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
        std::shared_ptr<DescriptorComponents> m_descriptor_components;

        std::shared_ptr<DeviceComponents> m_device_components;
        std::vector<VkDescriptorSet> m_descriptor_sets;

        void create_descriptor_sets(size_t count);

    public:
        DescriptorSetComponents(
            const std::shared_ptr<DeviceComponents> &device_components,
            const std::shared_ptr<DescriptorComponents> &descriptor_components);

        ~DescriptorSetComponents();

        VkDescriptorSet get_descriptor_set(size_t index) { return m_descriptor_sets[index]; }
    };

    class UniformBufferDescriptorSet: public DescriptorSetComponents
    {
    public:
        UniformBufferDescriptorSet(
            const std::shared_ptr<DeviceComponents> &device_components,
            const std::shared_ptr<DescriptorComponents> &descriptor_components,
            const std::vector<std::shared_ptr<BufferCPUtoGPU>> &uniform_buffers,
            size_t object_size);
    };
}