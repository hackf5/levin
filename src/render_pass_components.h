#pragma once

#include <vector>

#include "device_components.h"
#include "render_pass_factory.h"
#include "swapchain_components.h"

namespace levin
{
    class RenderPassComponents
    {
    private:
        RenderPassFactory m_factory;

        VkRenderPass m_render_pass;

        VkRenderPass create_render_pass(const SwapchainComponents &swapchain);

    public:
        RenderPassComponents(
            const DeviceComponents &device_components,
            const SwapchainComponents &swapchain_components);
        RenderPassComponents(const RenderPassComponents &) = delete;

        operator VkRenderPass() const { return m_render_pass; }
    };
}