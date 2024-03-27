#include "cubes_one.h"

using namespace levin;

std::array<Vertex, 8> CubesOne::create_vertexes()
{
    std::array<Vertex, 8> result;
    for (size_t i = 0; i < result.size(); ++i)
    {
        auto position = glm::vec3(
            (i & 1) ? 1.0f : -1.0f,
            (i & 2) ? 1.0f : -1.0f,
            (i & 4) ? 1.0f : -1.0f);
        auto uv = glm::vec2(
            (i & 1) ? 1.0f : 0.0f,
            (i & 2) ? 1.0f : 0.0f);
        auto color = glm::vec4(
            (i & 1) ? 1.0f : 0.0f,
            (i & 2) ? 1.0f : 0.0f,
            (i & 4) ? 1.0f : 0.0f,
            1.0f);

        result[i] = Vertex { position, uv, color };
    }

    return result;
}

std::array<Vertex::index_t, 36> CubesOne::create_indexes()
{
    return {
        0, 1, 2, 2, 3, 0,
        4, 6, 5, 6, 4, 7,
        0, 4, 1, 4, 5, 1,
        2, 3, 6, 3, 7, 6,
        1, 5, 2, 5, 6, 2,
        0, 3, 4, 3, 7, 4
    };
}

void CubesOne::update(
    uint32_t frame_index,
    float aspect_ratio,
    float time)
{
}

void CubesOne::load(
    const Device &device,
    TextureFactory &texture_factory,
    GraphicsBuffers &graphics_buffers)
{
}