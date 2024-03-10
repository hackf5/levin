#pragma once

#include <vector>

#include "vulkan_factory.h"
#include "device_components.h"
#include "swapchain_components.h"
#include "render_pass_components.h"

namespace levin
{
    class FramebufferComponents
    {
    private:
        bool m_initialized = false;

        VulkanFactory m_factory;

        std::vector<VkFramebuffer> m_framebuffers;

    public:
        FramebufferComponents(
            const DeviceComponents &device_components,
            const SwapchainComponents &swapchain_components,
            const RenderPassComponents &render_pass_components);
        FramebufferComponents(const FramebufferComponents &) = delete;

        VkFramebuffer framebuffer(uint32_t index) const { return m_framebuffers[index]; }
    };
}