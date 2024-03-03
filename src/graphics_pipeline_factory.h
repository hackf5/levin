#pragma once

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class GraphicsPipelineFactory: public VulkanFactory
    {
    private:
        std::vector<char> read_file(const std::string &name);

    public:
        GraphicsPipelineFactory(vkb::Device &device);

        VkShaderModule create_shader_module(const std::string &name);

        VkPipelineLayout create_pipeline_layout(const VkPipelineLayoutCreateInfo &create_info);

        VkPipeline create_pipeline(const VkGraphicsPipelineCreateInfo &create_info);

        void destroy_shader_module(VkShaderModule shader_module);
    };
}