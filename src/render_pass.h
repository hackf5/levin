#pragma once

#include "util/no_default_ctors.h"
#include "device.h"
#include "swapchain.h"

namespace levin
{
    class RenderPass : NoCopyOrMove
    {
    private:
        const Device &m_device;
        const Swapchain &m_swapchain;

        const VkRenderPass m_render_pass;

        VkRenderPass create_render_pass();

    public:
        RenderPass(
            const Device &device,
            const Swapchain &swapchain);
        ~RenderPass();

        operator VkRenderPass() const { return m_render_pass; }

        void begin(VkCommandBuffer command_buffer, VkFramebuffer framebuffer) const;

        void end(VkCommandBuffer command_buffer) const { vkCmdEndRenderPass(command_buffer); }
    };
}