
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vulkan/vulkan.h>

#include "vertex.h"
#include "device_components.h"
#include "descriptor_pool_components.h"
#include "descriptor_set_layout.h"
#include "descriptor_set_components.h"
#include "buffer.h"
#include "buffer_transfer_queue.h"

namespace levin
{
    class Camera
    {
    private:
        glm::mat4 m_view;
        glm::mat4 m_projection;

    public:
        Camera(
            glm::mat4 view,
            glm::mat4 projection):
            m_view(view),
            m_projection(projection)
        {
        }

        const glm::mat4 &view() const { return m_view; }
        glm::mat4 &view() { return m_view; }

        const glm::mat4 &projection() const { return m_projection; }
        glm::mat4 &projection() { return m_projection; }

        glm::mat4 matrix() const { return m_projection * m_view; }

        static glm::mat4 default_proj()
        {
            auto m = glm::perspective(
                glm::radians(45.0f),
                800.0f / 600.0f,
                0.1f,
                10.0f);
            m[1][1] *= -1;

            return m;
        }

        static Camera &default_camera()
        {
            static Camera camera(
                glm::lookAt(
                    glm::vec3(2.0f, 2.0f, 2.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 1.0f)),
                default_proj());

            return camera;
        }
    };

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

        void draw(VkCommandBuffer command_buffer) const
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
            glm::mat4 view;
            glm::mat4 proj;
        } m_uniform_block;

        // although raw pointers are bad juju, shared pointers are worse.
        // primitives will be shared between meshes.
        std::vector<Primitive *> m_primitives;

        // one for each frame in flight
        std::vector<std::unique_ptr<BufferCPUtoGPU>> m_uniform_buffers;
        std::vector<std::unique_ptr<UniformBufferDescriptorSet>> m_descriptor_sets;

        std::vector<std::unique_ptr<BufferCPUtoGPU>> create_uniform_buffers(const DeviceComponents &device);

        std::vector<std::unique_ptr<UniformBufferDescriptorSet>> create_descriptor_sets(
            const DeviceComponents &device,
            const DescriptorPoolComponents &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout);

    public:
        Mesh(
            const DeviceComponents &device,
            const DescriptorPoolComponents &descriptor_pool,
            const DescriptorSetLayout &descriptor_set_layout,
            std::vector<Primitive *> primitives);

        Mesh(const Mesh &) = delete;

        const glm::mat4 &model() const { return m_uniform_block.model; }
        glm::mat4 &model() { return m_uniform_block.model; }

        void flush(int current_frame)
        {
            UniformBlock block {
                m_uniform_block.model,
                Camera::default_camera().view(),
                Camera::default_camera().projection()
            };
            m_uniform_buffers[current_frame]->copy_from(&block, sizeof(m_uniform_block));
        }

        void draw(
            VkCommandBuffer command_buffer,
            const GraphicsPipelineComponents &pipeline,
            size_t current_frame) const
        {
            m_descriptor_sets[current_frame]->bind(command_buffer, pipeline);
            for (auto &primitive : m_primitives)
            {
                primitive->draw(command_buffer);
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

        void update(int current_frame = 0)
        {
            if (m_mesh)
            {
                m_mesh->model() = global_matrix();
                m_mesh->flush(current_frame);
            }

            for (auto &child : m_children)
            {
                child->update();
            }
        }

        void draw(
            VkCommandBuffer command_buffer,
            const GraphicsPipelineComponents &pipeline,
            size_t current_frame) const
        {
            if (m_mesh)
            {
                m_mesh->draw(command_buffer, pipeline, current_frame);
            }

            for (auto &child : m_children)
            {
                child->draw(command_buffer, pipeline, current_frame);
            }
        }
    };

    class Model
    {
    private:
        const DeviceComponents &m_device;
        const DescriptorPoolComponents &m_descriptor_pool;
        const BufferTransferQueue &m_transfer_queue;

        size_t m_vertex_count;
        std::unique_ptr<BufferGPU> m_vertex_buffer;

        size_t m_index_count;
        std::unique_ptr<BufferGPU> m_index_buffer;

        std::vector<std::unique_ptr<Primitive>> m_primitives;

        std::unique_ptr<Node> m_root_node;

    public:
        Model(
            const DeviceComponents &device,
            const DescriptorPoolComponents &descriptor_pool,
            const BufferTransferQueue &transfer_queue);

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

        void root_node(std::unique_ptr<Node> root_node) { m_root_node = std::move(root_node); }
        Node *root_node() { return m_root_node.get(); }

        void bind(VkCommandBuffer command_buffer) const
        {
            VkBuffer vertex_buffers[] = { *m_vertex_buffer };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(command_buffer, 0, 1, vertex_buffers, offsets);
            vkCmdBindIndexBuffer(command_buffer, *m_index_buffer, 0, Vertex::vk_index_type);
        }

        void draw(
            VkCommandBuffer command_buffer,
            const GraphicsPipelineComponents &pipeline,
            size_t current_frame) const
        {
            m_root_node->draw(command_buffer, pipeline, current_frame);
        }
    };
}