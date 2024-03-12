#include "shader_module_components.h"

#include "spdlog/spdlog.h"

using namespace levin;

ShaderModuleComponents::ShaderModuleComponents(const vkb::Device &device):
    m_factory(device)
{
}

void ShaderModuleComponents::load(const std::string &name)
{
    if (m_shader_modules.find(name) != m_shader_modules.end())
    {
        spdlog::warn("Shader Module already loaded: {}", name);
        return;
    }

    m_shader_modules[name] = m_factory.create_shader_module(name);
}

VkShaderModule ShaderModuleComponents::get(const std::string &name) const
{
    auto it = m_shader_modules.find(name);
    if (it == m_shader_modules.end())
    {
        throw std::runtime_error("Shader Module " + name + " not found");
    }

    return it->second;
}