#pragma once

#include <map>
#include <string>
#include <vulkan/vulkan.h>

#include "VkBootstrap.h"

#include "shader_module_factory.h"

namespace levin
{
    class ShaderModuleComponents
    {
    private:
        ShaderModuleFactory m_factory;

        std::map<std::string, VkShaderModule> m_shader_modules;

    public:
        ShaderModuleComponents(const vkb::Device &device);
        ShaderModuleComponents(const ShaderModuleComponents &) = delete;

        void load(const std::string &name);

        VkShaderModule get(const std::string &name) const;
    };
}