#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "device.h"
#include "descriptor_pool.h"
#include "descriptor_set_layout.h"
#include "graphics_pipeline.h"
#include "buffer.h"

namespace levin
{
    class DescriptorSet
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
        DescriptorSet(const DescriptorSet &) = delete;
        ~DescriptorSet();

        operator VkDescriptorSet() const { return m_descriptor_set; }
    };

    class UniformBufferDescriptorSet: public DescriptorSet
    {
    public:
        UniformBufferDescriptorSet(
            const Device &device,
            const DescriptorPool &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout,
            const Buffer &uniform_buffer);

        UniformBufferDescriptorSet(const UniformBufferDescriptorSet &) = delete;

        void bind(
            VkCommandBuffer command_buffer,
            const GraphicsPipeline &pipeline) const
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