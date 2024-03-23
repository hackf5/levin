#pragma once

#include "buffer.h"
#include "descriptor_set.h"

namespace levin
{
    class UniformBuffer: public BufferCPUtoGPU
    {
    public:
        typedef enum
        {
            CAMERA = 0,
            MESH = 1,
        } Usage;

    private:
        DescriptorSet m_descriptor_set;
        Usage m_usage;

    public:
        UniformBuffer(
            const Device &device,
            const DescriptorPool &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout,
            VkDeviceSize size,
            Usage usage);

        void bind(VkCommandBuffer command_buffer, const GraphicsPipeline &pipeline) const;
    };
}