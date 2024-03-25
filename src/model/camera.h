#pragma once

#include <array>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.h>

#include "util/no_copy_or_move.h"

#include "vulkan/buffer/buffer_host.h"

namespace levin
{
    class Camera: NoCopyOrMove
    {
    private:
        typedef std::array<std::unique_ptr<BufferHost>, Device::max_frames_in_flight> uniform_buffers_t;

        struct UniformBlock
        {
            glm::mat4 proj;
            glm::mat4 view;
        };

        uniform_buffers_t m_uniform_buffers;

        glm::vec3 m_position;
        glm::vec3 m_target;

        float m_fov;
        float m_aspect_ratio;
        float m_near;
        float m_far;

        bool m_dirty = true;

        UniformBlock m_uniform_block;

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
        Camera(const Device &device):
            m_uniform_buffers(create_uniform_buffers(device)),
            m_uniform_block({ glm::identity<glm::mat4>(), glm::identity<glm::mat4>() }),
            m_position(glm::zero<glm::vec3>()),
            m_target(glm::zero<glm::vec3>()),
            m_fov(45.0f),
            m_aspect_ratio(1.0f),
            m_near(0.1f),
            m_far(10.0f)
        {
        }

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

        void flush(uint16_t frame_index)
        {
            update();
            m_uniform_buffers[frame_index]->copy_from(&m_uniform_block, sizeof(UniformBlock));
        }

        void bind(uint16_t frame_index, GraphicsPipeline &pipeline) const
        {
            pipeline
                .descriptor_set_layout()
                .write_uniform_buffer(m_uniform_buffers[0]->descriptor(), 0);
        }
    };
}