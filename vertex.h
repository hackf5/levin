#pragma once

#include <vector>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace levin
{

    struct Vertex
    {
        typedef uint32_t index_t;

        static const VkIndexType vk_index_type = VK_INDEX_TYPE_UINT32;

        glm::vec2 position;
        glm::vec3 color;

        static VkVertexInputBindingDescription vertexInputBindingDescription;
        static std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;
        static VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
    };

    enum class VertexComponent { Position, Color };

    struct VertexInputState
    {
        VkVertexInputBindingDescription binding;
        std::vector<VkVertexInputAttributeDescription> attributes;
        VkPipelineVertexInputStateCreateInfo pipeline;

        VertexInputState(
            uint32_t binding,
            const std::vector<VertexComponent> components);

    private:
        VkVertexInputBindingDescription create_binding(uint32_t binding);

        VkVertexInputAttributeDescription create_attribute(
            uint32_t location,
            VertexComponent component);

        std::vector<VkVertexInputAttributeDescription> create_attributes(
            const std::vector<VertexComponent> components);

        VkPipelineVertexInputStateCreateInfo create_pipeline();
    };
}