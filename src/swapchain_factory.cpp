#include "swapchain_factory.h"

#include "spdlog/spdlog.h"

using namespace levin;

SwapchainConfig SwapchainFactory::config = {};

SwapchainFactory::SwapchainFactory(vkb::Device &device)
    : VulkanFactory(device)
{
}

vkb::Swapchain SwapchainFactory::create_swapchain(vkb::Device &device)
{
    vkb::SwapchainBuilder swapchain_builder { device };

    auto swapchain_ret = swapchain_builder.build();
    if (!swapchain_ret)
    {
        throw std::runtime_error("Failed to create swapchain: " + swapchain_ret.error().message());
    }

    auto swapchain = swapchain_ret.value();

    if (swapchain.image_count < 3)
    {
        throw std::runtime_error("Swapchain image count must be at least 3");
    }

    config.image_format = swapchain.image_format;
    config.color_space = swapchain.color_space;
    config.image_count = swapchain.image_count;
    config.image_usage = swapchain.image_usage_flags;
    config.present_mode = swapchain.present_mode;
    config.extent = swapchain.extent;

    return swapchain;
}

const SwapchainConfig& SwapchainFactory::get_config(vkb::Device *device)
{
    if (device)
    {
        // there are circular dependencies between the swapchain and the render pass
        // in that the framebuffers need the render pass and the render pass needs to know
        // about the swapchain's config
        auto swapchain = create_swapchain(*device);
        vkb::destroy_swapchain(swapchain);
    }

    return config;
}

vkb::Swapchain SwapchainFactory::create_swapchain()
{
    spdlog::info("Creating Swapchain");

    auto swapchain = create_swapchain(get_device());

    register_destruction([=, this]()
        {
            spdlog::info("Destroying Swapchain");
            vkb::destroy_swapchain(swapchain);
        });

    return swapchain;
}

std::vector<VkImageView> SwapchainFactory::create_image_views(vkb::Swapchain &swapchain)
{
    spdlog::info("Creating Image Views");

    auto image_views = swapchain.get_image_views().value();

    for (auto image_view : image_views)
    {
        register_destruction([=, this]()
            {
                spdlog::info("Destroying Image Views");
                {
                    vkDestroyImageView(get_device(), image_view, nullptr);
                }
            });
    }

    return image_views;

}

VkFramebuffer SwapchainFactory::create_framebuffer(const VkFramebufferCreateInfo &create_info)
{
    spdlog::info("Creating Framebuffer");

    VkFramebuffer framebuffer;
    if (vkCreateFramebuffer(get_device(), &create_info, nullptr, &framebuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create framebuffer");
    }

    register_destruction([=, this]()
        {
            spdlog::info("Destroying Framebuffer");
            vkDestroyFramebuffer(get_device(), framebuffer, nullptr);
        });

    return framebuffer;
}