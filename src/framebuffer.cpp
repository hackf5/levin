#include "framebuffer.h"

#include "spdlog/spdlog.h"

using namespace levin;

Framebuffer::Framebuffer(
    const Device &device,
    const Swapchain &swapchain,
    const RenderPass &render_pass):
    m_factory(device),
    m_framebuffers(create_framebuffers(swapchain, render_pass))
{
}

std::vector<VkFramebuffer> Framebuffer::create_framebuffers(
    const Swapchain &swapchain,
    const RenderPass &render_pass)
{
    std::vector<VkFramebuffer> framebuffers;
    framebuffers.resize(swapchain.image_count());

    for (size_t i = 0; i < swapchain.image_count(); i++)
    {
        VkImageView attachments[] = {
            swapchain.image_view(i)
        };

        VkFramebufferCreateInfo framebuffer_info = {};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = render_pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = attachments;
        framebuffer_info.width = swapchain.extent().width;
        framebuffer_info.height = swapchain.extent().height;
        framebuffer_info.layers = 1;

        auto framebuffer = m_factory.create_framebuffer(framebuffer_info);
        framebuffers[i] = framebuffer;
    }

    return framebuffers;
}