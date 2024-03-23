#pragma once

#include <vulkan/vulkan.h>

#include "node.h"

#include "util/no_copy_or_move.h"

namespace levin
{
    class Model: NoCopyOrMove
    {
    private:
        Node m_root_node;

    public:
        const Node &root_node() const { return m_root_node; }
        Node &root_node() { return m_root_node; }

        void render(
            VkCommandBuffer command_buffer,
            const GraphicsPipeline &pipeline) const
        {
            m_root_node.render(command_buffer, pipeline);
        }

        void flush()
        {
            m_root_node.flush();
        }
    };
}