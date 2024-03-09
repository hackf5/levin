#pragma once

#include <array>

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace levin
{
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription get_binding_description();

        static std::array<VkVertexInputAttributeDescription, 2> get_attribute_descriptions();
    };
}