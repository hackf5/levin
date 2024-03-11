#pragma once

#include <vector>

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class ShaderModuleFactory: public VulkanFactory
    {
    private:
        static std::vector<char> read_file(const std::string &name);

    public:
        ShaderModuleFactory(const vkb::Device &device);
        ShaderModuleFactory(const ShaderModuleFactory &) = delete;

        VkShaderModule create_shader_module(const std::string &name);
    };
}