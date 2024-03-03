#include "swapchain_factory.h"

#include <algorithm>

#include "spdlog/spdlog.h"

using namespace levin;

SwapchainFactory::SwapchainFactory(vkb::Device device)
    : m_device(device)
{
    spdlog::info("Creating Vulkan Factory");
}

SwapchainFactory::~SwapchainFactory()
{
    spdlog::info("Destroying Vulkan Factory");

    std::reverse(m_destruction_queue.begin(), m_destruction_queue.end());
    for (auto &destroy : m_destruction_queue)
    {
        destroy();
    }
}

vkb::Swapchain SwapchainFactory::create_swapchain()
{
    spdlog::info("Creating Swapchain");

    vkb::SwapchainBuilder swapchain_builder { m_device };

    auto swapchain_ret = swapchain_builder.build();
    if (!swapchain_ret)
    {
        throw std::runtime_error("Failed to create swapchain: " + swapchain_ret.error().message());
    }

    auto swapchain = swapchain_ret.value();

    m_destruction_queue.push_back([=, this]()
        {
            spdlog::info("Destroying Swapchain");
            vkb::destroy_swapchain(swapchain);
        });

    return swapchain;
}

VkFramebuffer SwapchainFactory::create_framebuffer(const VkFramebufferCreateInfo &create_info)
{
    spdlog::info("Creating Framebuffer");

    VkFramebuffer framebuffer;
    if (vkCreateFramebuffer(m_device, &create_info, nullptr, &framebuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create framebuffer");
    }

    m_destruction_queue.push_back([=, this]()
        {
            spdlog::info("Destroying Framebuffer");
            vkDestroyFramebuffer(m_device, framebuffer, nullptr);
        });

    return framebuffer;
}

VkRenderPass SwapchainFactory::create_render_pass(const VkRenderPassCreateInfo &create_info)
{
    spdlog::info("Creating Render Pass");

    VkRenderPass render_pass;
    if (vkCreateRenderPass(m_device, &create_info, nullptr, &render_pass) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create render pass");
    }

    m_destruction_queue.push_back([=, this]()
        {
            spdlog::info("Destroying Render Pass");
            vkDestroyRenderPass(m_device, render_pass, nullptr);
        });

    return render_pass;
}

void SwapchainFactory::register_image_view_destruction(const VkImageView &image_view)
{
    m_destruction_queue.push_back([=, this]()
        {
            spdlog::info("Destroying Image Views");
            {
                vkDestroyImageView(m_device, image_view, nullptr);
            }
        });

}