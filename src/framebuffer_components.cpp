#include "framebuffer_components.h"

#include "spdlog/spdlog.h"

using namespace levin;

FramebufferComponents::FramebufferComponents(
    const Device &device_components,
    const SwapchainComponents &swapchain_components,
    const RenderPassComponents &render_pass_components)
    : m_factory(device_components),
    m_framebuffers(create_framebuffers(swapchain_components, render_pass_components))
{
}

std::vector<VkFramebuffer> FramebufferComponents::create_framebuffers(
    const SwapchainComponents &swapchain_components,
    const RenderPassComponents &render_pass_components)
{
    std::vector<VkFramebuffer> framebuffers;
    framebuffers.resize(swapchain_components.image_count());

    for (size_t i = 0; i < swapchain_components.image_count(); i++)
    {
        VkImageView attachments[] = {
            swapchain_components.image_view(i)
        };

        VkFramebufferCreateInfo framebuffer_info = {};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = render_pass_components;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = swapchain_components.extent().width;
        framebuffer_info.height = swapchain_components.extent().height;
        framebuffer_info.layers = 1;

        auto framebuffer = m_factory.create_framebuffer(framebuffer_info);
        framebuffers[i] = framebuffer;
    }

    return framebuffers;
}