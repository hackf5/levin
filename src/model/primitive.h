#pragma once

#include <cstdint>
#include <vulkan/vulkan.h>

#include "vulkan/texture.h"

namespace levin
{
class Primitive
{
private:
    const uint32_t m_index_offset;
    const uint32_t m_index_count;
    Texture *m_texture;

public:
    Primitive(
        uint32_t index_offset,
        uint32_t index_count,
        Texture *texture):
        m_index_offset(index_offset),
        m_index_count(index_count),
        m_texture(texture)
    {
    }

    uint32_t index_offset() const { return m_index_offset; }
    uint32_t index_count() const { return m_index_count; }

    void render(VkCommandBuffer command_buffer, GraphicsPipeline &pipeline)
    {
        pipeline
            .descriptor_set_layout()
            .write_combined_image_sampler(m_texture->descriptor(), 2);

        pipeline.push_descriptor_set(command_buffer);

        vkCmdDrawIndexed(command_buffer, m_index_count, 1, m_index_offset, 0, 0);
    }
};
}