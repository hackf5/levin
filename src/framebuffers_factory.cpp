#include "framebuffers_factory.h"

namespace levin
{
    FramebuffersFactory::FramebuffersFactory(const vkb::Device &device)
        : VulkanFactory(device)
    {
    }

    VkFramebuffer FramebuffersFactory::create_framebuffer(const VkFramebufferCreateInfo &create_info)
    {
        VkFramebuffer framebuffer;
        if (vkCreateFramebuffer(device(), &create_info, nullptr, &framebuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer");
        }

        register_destruction([framebuffer](const vkb::Device &device)
            {
                vkDestroyFramebuffer(device, framebuffer, nullptr);
            });

        return framebuffer;
    }
}