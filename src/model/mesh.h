#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "util/no_copy_or_move.h"

#include "vulkan/device.h"
#include "vulkan/buffer/buffer_host.h"
#include "vulkan/graphics_pipeline.h"
#include "vulkan/image.h"

#include "primitive.h"


namespace levin
{
    class Mesh: NoCopyOrMove
    {
    private:
        struct UniformBlock
        {
            glm::mat4 model;
        };

        UniformBlock m_uniform_block;
        std::vector<Primitive> m_primitives;
        std::unique_ptr<BufferHost> m_uniform_buffer;
        Image* m_texture;

    public:
        Mesh(
            const Device &device,
            const std::vector<Primitive> &primitives,
            Image* texture = nullptr):
            m_uniform_block {},
            m_primitives(primitives),
            m_texture(texture),
            m_uniform_buffer(std::make_unique<BufferHost>(
                device,
                sizeof(m_uniform_block),
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT))
        {
        }

        const glm::mat4 &model() const { return m_uniform_block.model; }
        glm::mat4 &model() { return m_uniform_block.model; }

        void flush()
        {
            m_uniform_buffer->copy_from(&m_uniform_block, sizeof(m_uniform_block));
        }

        void render(
            VkCommandBuffer command_buffer,
            GraphicsPipeline &pipeline) const
        {
            pipeline
                .descriptor_set_layout()
                .write_uniform_buffer(m_uniform_buffer->descriptor(), 1);

            if (m_texture)
            {
                pipeline
                    .descriptor_set_layout()
                    .write_combined_image_sampler(m_texture->descriptor(), 2);
            }

            pipeline.push_descriptor_set(command_buffer);

            for (auto &primitive : m_primitives)
            {
                primitive.render(command_buffer);
            }
        }
    };
}