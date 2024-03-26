#pragma once

#include <chrono>

#include "util/no_copy_or_move.h"
#include "model/scene.h"
#include "vulkan/device.h"
#include "vulkan/graphics_buffers.h"
#include "vulkan/graphics_pipeline.h"
#include "vulkan/texture_factory.h"

namespace levin
{
    class RenderScene : NoCopyOrMove
    {
    protected:
        Scene &m_scene;

        float time_delta()
        {
            static auto start_time = std::chrono::high_resolution_clock::now();
            auto current_time = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<float, std::chrono::seconds::period>(current_time - start_time).count();
        }

        virtual void update(
            uint32_t frame_index,
            float aspect_ratio,
            float time) = 0;

    public:
        RenderScene(
            Scene &scene):
            m_scene(scene)
        {
        }

        virtual void load(
            const Device &device,
            TextureFactory &texture_factory,
            GraphicsBuffers &graphics_buffers) = 0;

        void update(
            uint32_t frame_index,
            float aspect_ratio)
        {
            auto time = time_delta();
            update(frame_index, aspect_ratio, time);
        }

        void render(
            VkCommandBuffer command_buffer,
            uint32_t frame_index,
            GraphicsPipeline &pipeline)
        {
            m_scene.render(command_buffer, frame_index, pipeline);
        }
    };
}