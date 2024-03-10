#include "graphics_pipeline_factory.h"

#include <fstream>
#include "spdlog/spdlog.h"

using namespace levin;

GraphicsPipelineFactory::GraphicsPipelineFactory(const vkb::Device& device)
    : VulkanFactory(device)
{
}

std::vector<char> GraphicsPipelineFactory::read_file(const std::string &name)
{
    auto file_name = "shaders/" + name + ".spv";
    std::ifstream file(file_name, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        spdlog::error("Failed to open file: {} ({})", name, file_name);
        throw std::runtime_error("Failed to open file");
    }

    size_t file_size = (size_t)file.tellg();
    std::vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(file_size));

    file.close();

    return buffer;
}

VkShaderModule GraphicsPipelineFactory::create_shader_module(const std::string &name)
{
    spdlog::info("Creating Shader Module: {}", name);

    auto code = read_file(name);

    VkShaderModuleCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size();
    create_info.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shader_module;
    if (vkCreateShaderModule(device(), &create_info, nullptr, &shader_module) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create shader module");
    }

    return shader_module;
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

void GraphicsPipelineFactory::destroy_shader_module(VkShaderModule shader_module)
{
    spdlog::info("Destroying Shader Module");

    vkDestroyShaderModule(device(), shader_module, nullptr);
}
