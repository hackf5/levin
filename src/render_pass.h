#pragma once

#include "device.h"
#include "swapchain.h"

namespace levin
{
    class RenderPass
    {
    private:
        const Device &m_device;

        const VkRenderPass m_render_pass;

        VkRenderPass create_render_pass(const Swapchain &swapchain);

    public:
        RenderPass(
            const Device &device,
            const Swapchain &swapchain);
        RenderPass(const RenderPass &) = delete;
        ~RenderPass();

        operator VkRenderPass() const { return m_render_pass; }
    };
}