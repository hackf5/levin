#pragma once

#include <vector>

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    struct SwapchainConfig
    {
        uint32_t image_count;
        VkFormat image_format;
        VkImageUsageFlags image_usage;
        VkColorSpaceKHR color_space;
        VkPresentModeKHR present_mode;
        VkExtent2D extent;
    };

    class SwapchainFactory: public VulkanFactory
    {
    private:
        static SwapchainConfig config;
        static vkb::Swapchain create_swapchain(vkb::Device &device);

    public:
        SwapchainFactory(vkb::Device &device);

        vkb::Swapchain create_swapchain();
        std::vector<VkImageView> create_image_views(vkb::Swapchain &swapchain);
        VkFramebuffer create_framebuffer(const VkFramebufferCreateInfo &create_info);

        static const SwapchainConfig& get_config(vkb::Device *device = nullptr);
    };
}