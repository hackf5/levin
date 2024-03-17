#pragma once

#include <vector>

#include "device.h"
#include "render_pass_factory.h"
#include "swapchain_components.h"

namespace levin
{
    class RenderPass
    {
    private:
        RenderPassFactory m_factory;

        const VkRenderPass m_render_pass;

        VkRenderPass create_render_pass(const SwapchainComponents &swapchain);

    public:
        RenderPass(
            const Device &device_components,
            const SwapchainComponents &swapchain_components);
        RenderPass(const RenderPass &) = delete;

        operator VkRenderPass() const { return m_render_pass; }
    };
}