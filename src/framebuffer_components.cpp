#include "framebuffer_components.h"

#include "spdlog/spdlog.h"

using namespace levin;

FramebufferComponents::FramebufferComponents(
    const DeviceComponents &device_components,
    const SwapchainComponents &swapchain_components,
    const RenderPassComponents &render_pass_components)
    : m_factory(device_components)
{
    m_framebuffers.resize(swapchain_components.image_count());

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

        VkFramebuffer framebuffer;
        if (vkCreateFramebuffer(device_components, &framebuffer_info, nullptr, &framebuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create framebuffer");
        }

        m_factory.register_destruction([=, this](const vkb::Device & device)
            {
                spdlog::info("Destroying Framebuffer");
                vkDestroyFramebuffer(device, framebuffer, nullptr);
            });

        m_framebuffers[i] = framebuffer;
    }
}