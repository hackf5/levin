#pragma once

#include <vector>

#include "device.h"
namespace levin
{
    class Swapchain
    {
    private:
        const Device &m_device;

        vkb::Swapchain m_swapchain;
        const std::vector<VkImage> m_swapchain_images;
        const std::vector<VkImageView> m_swapchain_image_views;
        const VkViewport m_viewport;
        const VkRect2D m_scissor;

        vkb::Swapchain create_swapchain();
        VkViewport create_viewport();
        VkRect2D create_scissor();

    public:
        Swapchain(const Device &device);
        Swapchain(const Swapchain &) = delete;
        ~Swapchain();

        operator VkSwapchainKHR() const { return m_swapchain.swapchain; }

        VkExtent2D extent() const { return m_swapchain.extent; }

        uint32_t image_count() const { return m_swapchain.image_count; }

        VkFormat image_format() const { return m_swapchain.image_format; }

        const VkViewport &viewport() const { return m_viewport; }

        const VkRect2D &scissor() const { return m_scissor; }

        const VkImage &image(size_t image_index) const { return m_swapchain_images[image_index]; }

        const VkImageView &image_view(size_t image_index) const { return m_swapchain_image_views[image_index]; }
    };
}