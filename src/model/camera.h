
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/no_copy_or_move.h"

#include "vulkan/device.h"
#include "vulkan/descriptor_pool.h"
#include "vulkan/descriptor_set_layout.h"
#include "vulkan/uniform_buffer.h"

namespace levin
{
    class Camera : NoCopyOrMove
    {
    private:
        UniformBuffer m_uniform_buffer;

        glm::vec3 m_position;
        glm::vec3 m_target;

        float m_fov;
        float m_aspect_ratio;
        float m_near;
        float m_far;

        bool m_dirty = true;

        struct UniformBlock
        {
            glm::mat4 proj;
            glm::mat4 view;
        } m_uniform_block;

        void update()
        {
            if (!m_dirty)
            {
                return;
            }

            m_uniform_block.proj = glm::perspective(
                glm::radians(m_fov),
                m_aspect_ratio,
                m_near,
                m_far);
            m_uniform_block.proj[1][1] *= -1;

            m_uniform_block.view = glm::lookAt(
                m_position,
                m_target,
                glm::vec3(0.0f, 0.0f, 1.0f));

            m_dirty = false;
        }

    public:
        Camera(
            const Device &device,
            const DescriptorPool &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout);

        const glm::vec3 &position() const { return m_position; }
        glm::vec3 &position() { m_dirty = true; return m_position; }

        const glm::vec3 &target() const { return m_target; }
        glm::vec3 &target() { m_dirty = true; return m_target; }

        float fov() const { return m_fov; }
        float &fov() { m_dirty = true; return m_fov; }

        float aspect_ratio() const { return m_aspect_ratio; }
        float &aspect_ratio() { m_dirty = true; return m_aspect_ratio; }

        float clip_near() const { return m_near; }
        float &clip_near() { m_dirty = true; return m_near; }

        float clip_far() const { return m_far; }
        float &clip_far() { m_dirty = true; return m_far; }

        const glm::mat4 &view() { update(); return m_uniform_block.view; }
        const glm::mat4 &proj() { update(); return m_uniform_block.proj; }

        void flush()
        {
            update();
            m_uniform_buffer.copy_from(&m_uniform_block, sizeof(UniformBlock));
        }

        void bind(VkCommandBuffer command_buffer, const GraphicsPipeline &pipeline) const
        {
            m_uniform_buffer.bind(command_buffer, pipeline);
        }
    };
}