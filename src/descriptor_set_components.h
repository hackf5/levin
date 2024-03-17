#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "device_components.h"
#include "descriptor_pool_components.h"
#include "descriptor_set_layout.h"
#include "graphics_pipeline_components.h"
#include "buffer.h"

namespace levin
{
    class DescriptorSetComponents
    {
    private:
        VkDescriptorSet create_descriptor_set(const DescriptorSetLayout &descriptor_set_layout);

    protected:
        const DeviceComponents &m_device;
        const DescriptorPoolComponents &m_descriptor_pool;
        const VkDescriptorSet m_descriptor_set;

    public:
        DescriptorSetComponents(
            const DeviceComponents &device,
            const DescriptorPoolComponents &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout);
        DescriptorSetComponents(const DescriptorSetComponents &) = delete;
        ~DescriptorSetComponents();

        operator VkDescriptorSet() const { return m_descriptor_set; }
    };

    class UniformBufferDescriptorSet: public DescriptorSetComponents
    {
    public:
        UniformBufferDescriptorSet(
            const DeviceComponents &device,
            const DescriptorPoolComponents &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout,
            const Buffer &uniform_buffer);

        UniformBufferDescriptorSet(const UniformBufferDescriptorSet &) = delete;

        void bind(
            VkCommandBuffer command_buffer,
            const GraphicsPipelineComponents &pipeline,
            size_t current_frame) const
        {
            vkCmdBindDescriptorSets(
                command_buffer,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipeline.layout(),
                0,
                1,
                &m_descriptor_set,
                0,
                nullptr);
        }
    };
}