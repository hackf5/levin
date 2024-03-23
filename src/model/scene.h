#pragma once

#include <vulkan/vulkan.h>

#include "util/no_copy_or_move.h"
#include "vulkan/buffer/uniform_buffer_factory.h"
#include "camera.h"
#include "model.h"

namespace levin
{
    class Scene : public NoCopyOrMove
    {
    private:
        Camera m_camera;
        Model m_model;

    public:
        Scene(UniformBufferFactory& uniform_buffer_factory)
            : m_camera(uniform_buffer_factory),
              m_model()
        {
        }

        const Camera &camera() const { return m_camera; }
        Camera &camera() { return m_camera; }

        const Model &model() const { return m_model; }
        Model &model() { return m_model; }

        void flush()
        {
            m_camera.flush();
            m_model.flush();
        }

        void bind(VkCommandBuffer command_buffer, const GraphicsPipeline &pipeline) const
        {
            m_camera.bind(command_buffer, pipeline);
        }

        void render(VkCommandBuffer command_buffer, const GraphicsPipeline &pipeline) const
        {
            m_model.render(command_buffer, pipeline);
        }
    };

}