#pragma once

#include <vector>

#include "util/no_copy_or_move.h"
#include "device.h"
#include "swapchain.h"
#include "render_pass.h"

namespace levin
{
    class Framebuffers : NoCopyOrMove
    {
    private:
        const Device &m_device;

        const std::vector<VkFramebuffer> m_framebuffers;

        std::vector<VkFramebuffer> create_framebuffers(
            const Swapchain &swapchain,
            const RenderPass &render_pass);

    public:
        Framebuffers(
            const Device &device,
            const Swapchain &swapchain,
            const RenderPass &render_pass);
        Framebuffers(const Framebuffers &) = delete;
        ~Framebuffers();

        VkFramebuffer get(uint32_t image_index) const { return m_framebuffers[image_index]; }
    };
}