#pragma once

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class GraphicsPipelineFactory: public VulkanFactory
    {
    public:
        GraphicsPipelineFactory(const vkb::Device &device);
        GraphicsPipelineFactory(const GraphicsPipelineFactory &) = delete;

        VkPipelineLayout create_pipeline_layout(const VkPipelineLayoutCreateInfo &create_info);

        VkPipeline create_pipeline(const VkGraphicsPipelineCreateInfo &create_info);
    };
}