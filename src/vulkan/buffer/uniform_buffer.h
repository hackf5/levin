#pragma once

#include <vulkan/vulkan.h>

#include "buffer_host.h"

#include "vulkan/device.h"
#include "vulkan/descriptor_pool.h"
#include "vulkan/descriptor_set.h"
#include "vulkan/descriptor_set_layout.h"
#include "vulkan/graphics_pipeline.h"

namespace levin
{
    class UniformBuffer: public BufferHost
    {
    public:
        typedef enum
        {
            CAMERA = 1,
            MESH = 2,
        } Usage;

    private:
        DescriptorSet m_descriptor_set;
        Usage m_usage;

    public:
        UniformBuffer(
            const Device &device,
            const DescriptorSetLayout &descriptor_set_layout,
            const DescriptorPool &descriptor_pool,
            VkDeviceSize size,
            Usage usage);

        void bind(VkCommandBuffer command_buffer, const GraphicsPipeline &pipeline) const;
    };
}