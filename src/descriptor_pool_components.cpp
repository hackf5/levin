#include "descriptor_pool_components.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

DescriptorPoolComponents::DescriptorPoolComponents(const DeviceComponents &device):
    m_factory(device),
    m_descriptor_set_layout(create_descriptor_set_layout()),
    m_descriptor_pool(create_descriptor_pool())
{
}

VkDescriptorSetLayout DescriptorPoolComponents::create_descriptor_set_layout()
{
    VkDescriptorSetLayoutBinding ubo_layout_binding = {};
    ubo_layout_binding.binding = 0;
    ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    ubo_layout_binding.descriptorCount = 1;
    ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    ubo_layout_binding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = 1;
    layout_info.pBindings = &ubo_layout_binding;

    return m_factory.create_descriptor_set_layout(layout_info);
}

VkDescriptorPool DescriptorPoolComponents::create_descriptor_pool()
{
    VkDescriptorPoolSize pool_size = {};
    pool_size.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pool_size.descriptorCount = static_cast<uint32_t>(DeviceComponents::max_frames_in_flight);

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.poolSizeCount = 1;
    pool_info.pPoolSizes = &pool_size;
    pool_info.maxSets = static_cast<uint32_t>(DeviceComponents::max_frames_in_flight);

    return m_factory.create_descriptor_pool(pool_info);
}