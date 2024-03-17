#pragma once

#include <vector>

#include "framebuffers_factory.h"
#include "device.h"
#include "swapchain_components.h"
#include "render_pass.h"

namespace levin
{
    class Framebuffer
    {
    private:
        bool m_initialized = false;

        FramebuffersFactory m_factory;

        const std::vector<VkFramebuffer> m_framebuffers;

        std::vector<VkFramebuffer> create_framebuffers(
            const SwapchainComponents &swapchain_components,
            const RenderPass &render_pass);

    public:
        Framebuffer(
            const Device &device_components,
            const SwapchainComponents &swapchain_components,
            const RenderPass &render_pass);
        Framebuffer(const Framebuffer &) = delete;

        VkFramebuffer framebuffer(uint32_t index) const { return m_framebuffers[index]; }
    };
}