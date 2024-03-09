#pragma once

#include <array>
#include <type_traits>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace levin
{
    struct Vertex
    {
        typedef uint16_t index_t;

        static const VkIndexType vk_index_type = std::is_same<index_t, uint16_t>::value
            ? VK_INDEX_TYPE_UINT16
            : VK_INDEX_TYPE_UINT32;

        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription get_binding_description();

        static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions();
    };
}