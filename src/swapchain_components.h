#pragma once

#include <vector>

#include "device_components.h"
#include "swapchain_factory.h"

namespace levin
{
    struct SwapchainComponents
    {
        SwapchainFactory factory;

        vkb::Swapchain swapchain;
        VkRenderPass render_pass;
        std::vector<VkImage> swapchain_images;
        std::vector<VkImageView> swapchain_image_views;
        std::vector<VkFramebuffer> framebuffers;

        SwapchainComponents(DeviceComponents &components);
        ~SwapchainComponents();

    private:
        void init_render_pass(DeviceComponents &components);

        void init_framebuffers(DeviceComponents &components);
    };
}