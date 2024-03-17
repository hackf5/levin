#pragma once

#include <map>
#include <string>
#include <vulkan/vulkan.h>

#include "VkBootstrap.h"

#include "shader_module_factory.h"

namespace levin
{
    class ShaderModule
    {
    private:
        ShaderModuleFactory m_factory;

        std::map<std::string, VkShaderModule> m_shader_modules;

    public:
        ShaderModule(const vkb::Device &device);
        ShaderModule(const ShaderModule &) = delete;

        void load(const std::string &name);

        VkShaderModule get(const std::string &name) const;
    };
}