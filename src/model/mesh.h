#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "util/no_copy_or_move.h"

#include "vulkan/device.h"
#include "vulkan/buffer/buffer_host.h"
#include "vulkan/graphics_pipeline.h"
#include "vulkan/texture.h"

#include "primitive.h"


namespace levin
{
    class Mesh: NoCopyOrMove
    {
    private:
        typedef std::array<std::unique_ptr<BufferHost>, Device::max_frames_in_flight> uniform_buffers_t;

        struct UniformBlock
        {
            glm::mat4 model;
        };

        UniformBlock m_uniform_block;
        std::vector<Primitive> m_primitives;
        Texture *m_texture;
        uniform_buffers_t m_uniform_buffers;

        uniform_buffers_t create_uniform_buffers(const Device &device)
        {
            uniform_buffers_t uniform_buffers;
            for (auto &uniform_buffer : uniform_buffers)
            {
                uniform_buffer = std::make_unique<BufferHost>(
                    device,
                    sizeof(UniformBlock),
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
            }

            return uniform_buffers;
        }

    public:
        Mesh(
            const Device &device,
            const std::vector<Primitive> &primitives,
            Texture *texture = nullptr):
            m_uniform_block {},
            m_primitives(primitives),
            m_texture(texture),
            m_uniform_buffers(create_uniform_buffers(device))
        {
        }

        const glm::mat4 &model() const { return m_uniform_block.model; }
        glm::mat4 &model() { return m_uniform_block.model; }

        void flush(uint32_t frame_index)
        {
            m_uniform_buffers[frame_index]->copy_from(&m_uniform_block, sizeof(m_uniform_block));
        }

        void render(
            VkCommandBuffer command_buffer,
            uint32_t frame_index,
            GraphicsPipeline &pipeline) const
        {
            pipeline
                .descriptor_set_layout()
                .write_uniform_buffer(m_uniform_buffers[frame_index]->descriptor(), 1);

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