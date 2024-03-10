#pragma once

#include <vector>

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class SwapchainFactory: public VulkanFactory
    {
    private:
        static vkb::Swapchain create_swapchain(const vkb::Device &device);

    public:
        SwapchainFactory(const vkb::Device &device);
        SwapchainFactory(const SwapchainFactory &) = delete;

        vkb::Swapchain create_swapchain();
        std::vector<VkImageView> create_image_views(vkb::Swapchain &swapchain);
    };
}