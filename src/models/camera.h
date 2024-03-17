
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "device.h"
#include "descriptor_pool.h"
#include "descriptor_set_layout.h"
#include "uniform_buffer.h"

namespace levin
{
    class Camera
    {
    private:
        UniformBuffer m_uniform_buffer;

        glm::vec3 m_position;
        glm::vec3 m_target;

        float m_fov;
        float m_aspect_ratio;
        float m_near;
        float m_far;

        struct UniformBlock
        {
            glm::mat4 proj;
            glm::mat4 view;
        } m_uniform_block;

        void update_projection()
        {
            m_uniform_block.proj = glm::perspective(
                glm::radians(m_fov),
                m_aspect_ratio,
                m_near,
                m_far);
            m_uniform_block.proj[1][1] *= -1;
        }

        void update_view()
        {
            m_uniform_block.view = glm::lookAt(
                m_position,
                m_target,
                glm::vec3(0.0f, 0.0f, 1.0f));
        }

    public:
        Camera(
            const Device &device,
            const DescriptorPool &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout);

        const glm::vec3 &position() const { return m_position; }
        glm::vec3 &position() { return m_position; }

        const glm::vec3 &target() const { return m_target; }
        glm::vec3 &target() { return m_target; }

        float fov() const { return m_fov; }
        float& fov() { return m_fov; }

        float aspect_ratio() const { return m_aspect_ratio; }
        float& aspect_ratio() { return m_aspect_ratio; }

        float clip_near() const { return m_near; }
        float& clip_near() { return m_near; }

        float clip_far() const { return m_far; }
        float& clip_far() { return m_far; }

        const glm::mat4 &view() const { return m_uniform_block.view; }
        const glm::mat4 &proj() const { return m_uniform_block.proj; }

        void flush()
        {
            update_view();
            update_projection();
            m_uniform_buffer.copy_from(&m_uniform_block, sizeof(UniformBlock));
        }

        void bind(VkCommandBuffer command_buffer, const GraphicsPipeline &pipeline) const
        {
            m_uniform_buffer.bind(command_buffer, pipeline);
        }
    };
}