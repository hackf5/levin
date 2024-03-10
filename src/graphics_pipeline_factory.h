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
        GraphicsPipelineFactory(const vkb::Device &device);
        GraphicsPipelineFactory(const GraphicsPipelineFactory &) = delete;

        VkShaderModule create_shader_module(const std::string &name);

        VkPipelineLayout create_pipeline_layout(const VkPipelineLayoutCreateInfo &create_info);

        VkDescriptorSetLayout create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo &create_info);

        VkDescriptorPool create_descriptor_pool(const VkDescriptorPoolCreateInfo &create_info);

        VkPipeline create_pipeline(const VkGraphicsPipelineCreateInfo &create_info);

        void destroy_shader_module(VkShaderModule shader_module);
    };
}