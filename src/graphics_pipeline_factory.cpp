#include "graphics_pipeline_factory.h"

#include <fstream>
#include "spdlog/spdlog.h"

using namespace levin;

GraphicsPipelineFactory::GraphicsPipelineFactory(const vkb::Device &device)
    : VulkanFactory(device)
{
}

VkPipelineLayout GraphicsPipelineFactory::create_pipeline_layout(const VkPipelineLayoutCreateInfo &create_info)
{
    spdlog::info("Creating Pipeline Layout");

    VkPipelineLayout pipeline_layout;
    if (vkCreatePipelineLayout(device(), &create_info, nullptr, &pipeline_layout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }

    register_destruction([pipeline_layout](const vkb::Device &device)
        {
            spdlog::info("Destroying Pipeline Layout");
            vkDestroyPipelineLayout(device, pipeline_layout, nullptr);
        });

    return pipeline_layout;
}

VkPipeline GraphicsPipelineFactory::create_pipeline(const VkGraphicsPipelineCreateInfo &create_info)
{
    spdlog::info("Creating Pipeline");

    VkPipeline pipeline;
    if (vkCreateGraphicsPipelines(device(), VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline");
    }

    register_destruction([pipeline](const vkb::Device &device)
        {
            spdlog::info("Destroying Graphics Pipeline");
            vkDestroyPipeline(device, pipeline, nullptr);
        });

    return pipeline;
}

VkDescriptorSetLayout GraphicsPipelineFactory::create_descriptor_set_layout(const VkDescriptorSetLayoutCreateInfo &create_info)
{
    spdlog::info("Creating Descriptor Set Layout");

    VkDescriptorSetLayout descriptor_set_layout;
    if (vkCreateDescriptorSetLayout(device(), &create_info, nullptr, &descriptor_set_layout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor set layout");
    }

    register_destruction([descriptor_set_layout](const vkb::Device &device)
        {
            spdlog::info("Destroying Descriptor Set Layout");
            vkDestroyDescriptorSetLayout(device, descriptor_set_layout, nullptr);
        });

    return descriptor_set_layout;
}

VkDescriptorPool GraphicsPipelineFactory::create_descriptor_pool(const VkDescriptorPoolCreateInfo &create_info)
{
    spdlog::info("Creating Descriptor Pool");

    VkDescriptorPool descriptor_pool;
    if (vkCreateDescriptorPool(device(), &create_info, nullptr, &descriptor_pool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }

    register_destruction([descriptor_pool](const vkb::Device &device)
        {
            spdlog::info("Destroying Descriptor Pool");
            vkDestroyDescriptorPool(device, descriptor_pool, nullptr);
        });

    return descriptor_pool;
}