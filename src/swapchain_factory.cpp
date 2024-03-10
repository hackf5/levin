#include "swapchain_factory.h"

#include "spdlog/spdlog.h"

using namespace levin;

SwapchainFactory::SwapchainFactory(const vkb::Device &device)
    : VulkanFactory(device)
{
}

vkb::Swapchain SwapchainFactory::create_swapchain()
{
    spdlog::info("Creating Swapchain");

    vkb::SwapchainBuilder swapchain_builder { device() };

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

    register_destruction([swapchain](const vkb::Device & device)
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
        register_destruction([image_view](const vkb::Device & device)
            {
                spdlog::info("Destroying Image Views");
                {
                    vkDestroyImageView(device, image_view, nullptr);
                }
            });
    }

    return image_views;

}