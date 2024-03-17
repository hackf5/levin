#include "descriptor_set_layout.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorSetLayout::DescriptorSetLayout(const Device &device):
    m_device(device),
    m_descriptor_set_layout(create_descriptor_set_layout())
{
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    vkDestroyDescriptorSetLayout(m_device, m_descriptor_set_layout, nullptr);
}

VkDescriptorSetLayout DescriptorSetLayout::create_descriptor_set_layout()
{
    // currently specific to uniform buffer
    VkDescriptorSetLayoutBinding uniform_layout_binding = {};
    uniform_layout_binding.binding = 0;
    uniform_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniform_layout_binding.descriptorCount = 1;
    uniform_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uniform_layout_binding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 1;
    layout_info.pBindings = &uniform_layout_binding;

    VkDescriptorSetLayout descriptor_set_layout;
    if (vkCreateDescriptorSetLayout(m_device, &layout_info, nullptr, &descriptor_set_layout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor set layout");
    }

    return descriptor_set_layout;
}