#pragma once

#include <vector>

#include "device.h"
#include "swapchain_factory.h"

namespace levin
{
    class SwapchainComponents
    {
    private:
        SwapchainFactory m_factory;

        vkb::Swapchain m_swapchain;
        const VkViewport m_viewport;
        const VkRect2D m_scissor;

        const std::vector<VkImage> m_swapchain_images;
        const std::vector<VkImageView> m_swapchain_image_views;

        VkViewport create_viewport();
        VkRect2D create_scissor();

    public:
        SwapchainComponents(const Device &device_components);
        SwapchainComponents(const SwapchainComponents &) = delete;

        operator VkSwapchainKHR() const { return m_swapchain.swapchain; }

        VkExtent2D extent() const { return m_swapchain.extent; }

        uint32_t image_count() const { return m_swapchain.image_count; }

        VkFormat image_format() const { return m_swapchain.image_format; }

        const VkViewport &viewport() const { return m_viewport; }

        const VkRect2D &scissor() const { return m_scissor; }

        const VkImage &image(size_t index) const { return m_swapchain_images[index]; }

        const VkImageView &image_view(size_t index) const { return m_swapchain_image_views[index]; }
    };
}