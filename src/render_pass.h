#pragma once

#include <vector>

#include "device.h"
#include "render_pass_factory.h"
#include "swapchain.h"

namespace levin
{
    class RenderPass
    {
    private:
        RenderPassFactory m_factory;

        const VkRenderPass m_render_pass;

        VkRenderPass create_render_pass(const Swapchain &swapchain);

    public:
        RenderPass(
            const Device &device_components,
            const Swapchain &swapchain);
        RenderPass(const RenderPass &) = delete;

        operator VkRenderPass() const { return m_render_pass; }
    };
}