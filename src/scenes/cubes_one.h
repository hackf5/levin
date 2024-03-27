#pragma once

#include <array>

#include <glm/glm.hpp>

#include "util/no_copy_or_move.h"
#include "render_scene.h"
#include "vulkan/device.h"
#include "vulkan/graphics_buffers.h"
#include "vulkan/graphics_pipeline.h"
#include "vulkan/texture_factory.h"
#include "vulkan/vertex.h"

namespace levin
{
class CubesOne: public RenderScene
{
    private:
    std::array<Vertex, 8> m_vertexes;
    std::array<Vertex::index_t, 36> m_indexes;

    std::array<Vertex, 8> create_vertexes();
    std::array<Vertex::index_t, 36> create_indexes();

protected:
    void update(
        uint32_t frame_index,
        float aspect_ratio,
        float time)
        override;

public:
    CubesOne(const Device &device):
        RenderScene(device, "cubes.vert", "cubes.frag"),
        m_vertexes(create_vertexes()),
        m_indexes(create_indexes())
    {
    }

    virtual void load(
        const Device &device,
        TextureFactory &texture_factory,
        GraphicsBuffers &graphics_buffers)
        override;
};
}