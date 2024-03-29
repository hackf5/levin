#pragma once

#include <memory>
#include <string>
#include <vector>

#include "tiny_obj_loader.h"

#include "model/mesh.h"
#include "model/primitive.h"
#include "vulkan/device.h"
#include "vulkan/texture.h"
#include "vulkan/texture_factory.h"
#include "vulkan/vertex.h"

namespace levin
{
class MeshLoader
{
private:
    const Device &m_device;
    TextureFactory &m_texture_factory;

public:
    MeshLoader(const Device &device, TextureFactory &texture_factory):
        m_device(device),
        m_texture_factory(texture_factory)
    {
    }

    std::unique_ptr<Mesh> load(const std::string &filename)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
        {
            throw std::runtime_error(warn + err);
        }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)
            {
                Vertex vertex {};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                // if (index.normal_index >= 0)
                // {
                //     vertex.normal = {
                //         attrib.normals[3 * index.normal_index + 0],
                //         attrib.normals[3 * index.normal_index + 1],
                //         attrib.normals[3 * index.normal_index + 2]
                //     };
                // }

                if (index.texcoord_index >= 0)
                {
                    vertex.uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                vertices.push_back(vertex);
                indices.push_back(indices.size());
            }
        }

        std::vector<Primitive> primitives;
        primitives.emplace_back(m_device, vertices, indices);

        Texture *texture = nullptr;

        if (!materials.empty())
        {
            const auto &material = materials[0];

            // if (!material.diffuse_texname.empty())
            // {
            //     texture = m_texture_factory.load(material.diffuse_texname);
            // }
        }

        return std::make_unique<Mesh>(m_device, primitives, texture);
    }
};
}