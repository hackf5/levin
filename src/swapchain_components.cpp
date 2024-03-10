#include "swapchain_components.h"

#include "spdlog/spdlog.h"

using namespace levin;

SwapchainComponents::SwapchainComponents(
    const DeviceComponents &device_components,
    VkRenderPass render_pass):
    m_factory(SwapchainFactory(device_components)),
    m_swapchain(m_factory.create_swapchain()),
    m_swapchain_images(m_swapchain.get_images().value()),
    m_swapchain_image_views(m_factory.create_image_views(m_swapchain)),
    m_framebuffers(create_framebuffers(m_factory, m_swapchain, m_swapchain_images, m_swapchain_image_views, render_pass))
{
}

std::vector<VkFramebuffer> SwapchainComponents::create_framebuffers(
    SwapchainFactory &factory,
    const vkb::Swapchain &swapchain,
    const std::vector<VkImage> &swapchain_images,
    const std::vector<VkImageView> &swapchain_image_views,
    VkRenderPass render_pass)
{
    std::vector<VkFramebuffer> framebuffers;
    framebuffers.resize(swapchain_images.size());

    for (size_t i = 0; i < swapchain_images.size(); i++)
    {
        VkImageView attachments[] = {
            swapchain_image_views[i]
        };

        VkFramebufferCreateInfo framebuffer_info = {};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = swapchain.extent.width;
        framebuffer_info.height = swapchain.extent.height;
        framebuffer_info.layers = 1;

        auto framebuffer = factory.create_framebuffer(framebuffer_info);
        framebuffers[i] = framebuffer;
    }

    return framebuffers;
}