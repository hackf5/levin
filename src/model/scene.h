#pragma once

#include <vulkan/vulkan.h>

#include "util/no_copy_or_move.h"
#include "vulkan/device.h"
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
        Scene(const Device& device)
            : m_camera(device),
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

        void render(
            VkCommandBuffer command_buffer,
            GraphicsPipeline &pipeline) const
        {
            m_camera.bind(pipeline);
            m_model.render(command_buffer, pipeline);
        }
    };

}