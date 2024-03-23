#include "uniform_buffer.h"

#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

UniformBuffer::UniformBuffer(
    const Device &device,
    const DescriptorSetLayout &descriptor_set_layout,
    const DescriptorPool &descriptor_pool,
    VkDeviceSize size,
    Usage usage):
    BufferHost(device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT),
    m_descriptor_set(
        device,
        descriptor_pool,
        descriptor_set_layout),
    m_usage(usage)
{
    auto buffer_info = descriptor();

    VkWriteDescriptorSet descriptor_write {};
    descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptor_write.dstSet = m_descriptor_set;
    descriptor_write.dstBinding = 1;
    descriptor_write.dstArrayElement = 0;
    descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptor_write.descriptorCount = 1;
    descriptor_write.pBufferInfo = &buffer_info;

    vkUpdateDescriptorSets(device, 1, &descriptor_write, 0, nullptr);
}

void UniformBuffer::bind(VkCommandBuffer command_buffer, const GraphicsPipeline &pipeline) const
{
    VkDescriptorSet descriptor_set = m_descriptor_set;
    vkCmdBindDescriptorSets(
        command_buffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        pipeline.layout(),
        static_cast<uint32_t>(m_usage),
        1,
        &descriptor_set,
        0,
        nullptr);
}