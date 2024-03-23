#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "util/no_copy_or_move.h"

#include "primitive.h"

#include "vulkan/device.h"
#include "vulkan/descriptor_pool.h"
#include "vulkan/descriptor_set_layout.h"
#include "vulkan/graphics_pipeline.h"
#include "vulkan/buffer/uniform_buffer.h"

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

        UniformBuffer m_uniform_buffer;

    public:
        Mesh(
            const Device &device,
            const DescriptorPool &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout,
            const std::vector<Primitive> &primitives):
            m_uniform_block {},
            m_primitives(primitives),
            m_uniform_buffer(
            device,
            descriptor_pool,
            descriptor_set_layout,
            sizeof(m_uniform_block),
            UniformBuffer::MESH)
        {
        }


        const glm::mat4 &model() const { return m_uniform_block.model; }
        glm::mat4 &model() { return m_uniform_block.model; }

        void flush()
        {
            m_uniform_buffer.copy_from(&m_uniform_block, sizeof(m_uniform_block));
        }

        void render(
            VkCommandBuffer command_buffer,
            const GraphicsPipeline &pipeline) const
        {
            m_uniform_buffer.bind(command_buffer, pipeline);
            for (auto &primitive : m_primitives)
            {
                primitive.render(command_buffer);
            }
        }
    };
}