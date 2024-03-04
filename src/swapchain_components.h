#pragma once

#include <vector>

#include "device_components.h"
#include "swapchain_factory.h"

namespace levin
{
    class SwapchainComponents
    {
    private:
        SwapchainFactory m_factory;

        vkb::Swapchain m_swapchain;
        std::vector<VkImage> m_swapchain_images;
        std::vector<VkImageView> m_swapchain_image_views;
        std::vector<VkFramebuffer> m_framebuffers;

        static std::vector<VkFramebuffer> create_framebuffers(
            SwapchainFactory &factory,
            const vkb::Swapchain &swapchain,
            const std::vector<VkImage> &swapchain_images,
            const std::vector<VkImageView> &swapchain_image_views,
            VkRenderPass render_pass);

    public:
        SwapchainComponents(
            const std::shared_ptr<DeviceComponents> &device_components,
            VkRenderPass render_pass);

        VkSwapchainKHR get_swapchain() const
        {
            return m_swapchain.swapchain;
        }

        VkExtent2D get_extent() const
        {
            return m_swapchain.extent;
        }

        VkFramebuffer get_framebuffer(uint32_t index) const
        {
            return m_framebuffers[index];
        }
    };
}