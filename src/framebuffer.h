#pragma once

#include <vector>

#include "framebuffers_factory.h"
#include "device.h"
#include "swapchain.h"
#include "render_pass.h"

namespace levin
{
    class Framebuffer
    {
    private:
        FramebuffersFactory m_factory;

        const std::vector<VkFramebuffer> m_framebuffers;

        std::vector<VkFramebuffer> create_framebuffers(
            const Swapchain &swapchain,
            const RenderPass &render_pass);

    public:
        Framebuffer(
            const Device &device,
            const Swapchain &swapchain,
            const RenderPass &render_pass);
        Framebuffer(const Framebuffer &) = delete;

        VkFramebuffer framebuffer(uint32_t image_index) const { return m_framebuffers[image_index]; }
    };
}