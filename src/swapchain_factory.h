#pragma once

#include <vector>
#include <functional>

#include <vulkan/vulkan.h>

#include "VkBootstrap.h"

namespace levin
{
    class SwapchainFactory
    {
    private:
        std::vector<std::function<void()>> m_destruction_queue;

        vkb::Device m_device;

    public:
        SwapchainFactory(vkb::Device device);
        ~SwapchainFactory();

        vkb::Swapchain create_swapchain();

        VkFramebuffer create_framebuffer(const VkFramebufferCreateInfo &create_info);

        VkRenderPass create_render_pass(const VkRenderPassCreateInfo &create_info);

        void register_image_view_destruction(const VkImageView &image_view);
    };
}