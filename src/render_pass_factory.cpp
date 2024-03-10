#include "render_pass_factory.h"

#include "spdlog/spdlog.h"

using namespace levin;

RenderPassFactory::RenderPassFactory(const vkb::Device& device)
    : VulkanFactory(device)
{
}

VkRenderPass RenderPassFactory::create_render_pass(const VkRenderPassCreateInfo &create_info)
{
    spdlog::info("Creating Render Pass");

    VkRenderPass render_pass;
    if (vkCreateRenderPass(device(), &create_info, nullptr, &render_pass) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create render pass");
    }

    register_destruction([render_pass](const vkb::Device &device)
        {
            spdlog::info("Destroying Render Pass");
            vkDestroyRenderPass(device, render_pass, nullptr);
        });

    return render_pass;
}