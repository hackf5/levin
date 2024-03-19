
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vulkan/vulkan.h>

#include "vertex.h"
#include "device.h"
#include "descriptor_pool.h"
#include "descriptor_set_layout.h"
#include "transfer_queue.h"
#include "uniform_buffer.h"

namespace levin
{
    class Primitive
    {
    private:
        const uint32_t m_index_offset;
        const uint32_t m_index_count;

    public:
        Primitive(uint32_t index_offset, uint32_t index_count):
            m_index_offset(index_offset),
            m_index_count(index_count)
        {
        }

        uint32_t index_offset() const { return m_index_offset; }
        uint32_t index_count() const { return m_index_count; }

        void render(VkCommandBuffer command_buffer) const
        {
            vkCmdDrawIndexed(command_buffer, m_index_count, 1, m_index_offset, 0, 0);
        }
    };

    class Mesh
    {
    private:
        struct UniformBlock
        {
            glm::mat4 model;
        } m_uniform_block;

        // although raw pointers are bad juju, shared pointers are worse.
        // primitives will be shared between meshes.
        std::vector<Primitive *> m_primitives;

        UniformBuffer m_uniform_buffer;

    public:
        Mesh(
            const Device &device,
            const DescriptorPool &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout,
            std::vector<Primitive *> primitives);

        Mesh(const Mesh &) = delete;

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
                primitive->render(command_buffer);
            }
        }
    };

    class Node
    {
    private:
        Node *m_parent;
        std::vector<std::unique_ptr<Node>> m_children;
        std::unique_ptr<Mesh> m_mesh;

        glm::vec3 m_translation {};
        glm::vec3 m_scale { 1.0f };
        glm::quat m_rotation {};
        glm::mat4 m_matrix {};
        bool m_matrix_dirty = true;

    public:
        Node(
            Node *parent = nullptr,
            std::unique_ptr<Mesh> mesh = nullptr):
            m_parent(parent),
            m_mesh(std::move(mesh))
        {
        }

        Node(const Node &) = delete;

        Node *parent() const { return m_parent; }

        std::vector<std::unique_ptr<Node>> &children() { return m_children; }

        Node &add_child(std::unique_ptr<Mesh> mesh = nullptr)
        {
            auto &child = m_children.emplace_back(
                std::make_unique<Node>(this, std::move(mesh)));

            return *child;
        }

        const glm::vec3 &translation() const { return m_translation; }
        glm::vec3 &translation() { m_matrix_dirty = true; return m_translation; }

        const glm::vec3 &scale() const { return m_scale; }
        glm::vec3 &scale() { m_matrix_dirty = true; return m_scale; }

        const glm::quat &rotation() const { return m_rotation; }
        glm::quat &rotation() { m_matrix_dirty = true; return m_rotation; }

        const glm::mat4 &local_matrix()
        {
            if (m_matrix_dirty)
            {
                m_matrix = glm::translate(glm::mat4(1.0f), m_translation) *
                    glm::mat4_cast(m_rotation) *
                    glm::scale(glm::mat4(1.0f), m_scale);
                m_matrix_dirty = false;
            }

            return m_matrix;
        }

        glm::mat4 global_matrix()
        {
            return m_parent ? m_parent->global_matrix() * local_matrix() : local_matrix();
        }

        void flush()
        {
            if (m_mesh)
            {
                m_mesh->model() = global_matrix();
                m_mesh->flush();
            }

            for (auto &child : m_children)
            {
                child->flush();
            }
        }

        void render(
            VkCommandBuffer command_buffer,
            const GraphicsPipeline &pipeline) const
        {
            if (m_mesh)
            {
                m_mesh->render(command_buffer, pipeline);
            }

            for (auto &child : m_children)
            {
                child->render(command_buffer, pipeline);
            }
        }
    };

    class Model
    {
    private:
        const Device &m_device;
        const DescriptorPool &m_descriptor_pool;
        const TransferQueue &m_transfer_queue;

        std::unique_ptr<BufferGPU> m_vertex_buffer;

        std::unique_ptr<BufferGPU> m_index_buffer;

        std::vector<std::unique_ptr<Primitive>> m_primitives;

        std::unique_ptr<Node> m_root_node;

    public:
        Model(
            const Device &device,
            const DescriptorPool &descriptor_pool,
            const TransferQueue &transfer_queue);

        Model(const Model &) = delete;

        void load_vertexes(const std::vector<levin::Vertex> &vertexes);
        void load_indexes(const std::vector<Vertex::index_t> &indexes);
        void load_primitives(std::vector<std::unique_ptr<Primitive>> &primitives);

        std::vector<Primitive *> primitives() const
        {
            std::vector<Primitive *> primitives;
            for (auto &primitive : m_primitives)
            {
                primitives.push_back(primitive.get());
            }

            return primitives;
        }

        Node &root_node() { return *m_root_node; }

        void bind(VkCommandBuffer command_buffer) const
        {
            VkBuffer vertex_buffers[] = { *m_vertex_buffer };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
            vkCmdBindIndexBuffer(command_buffer, *m_index_buffer, 0, Vertex::vk_index_type);
        }

        void render(
            VkCommandBuffer command_buffer,
            const GraphicsPipeline &pipeline) const
        {
            m_root_node->render(command_buffer, pipeline);
        }

        void flush()
        {
            m_root_node->flush();
        }
    };
}