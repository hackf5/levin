#include "render_pass_factory.h"

#include "spdlog/spdlog.h"

using namespace levin;

RenderPassFactory::RenderPassFactory(vkb::Device& device)
    : VulkanFactory(device)
{
}

VkRenderPass RenderPassFactory::create_render_pass(const VkRenderPassCreateInfo &create_info)
{
    spdlog::info("Creating Render Pass");

    VkRenderPass render_pass;
    if (vkCreateRenderPass(get_device(), &create_info, nullptr, &render_pass) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create render pass");
    }

    register_destruction([=, this]()
        {
            spdlog::info("Destroying Render Pass");
            vkDestroyRenderPass(get_device(), render_pass, nullptr);
        });

    return render_pass;
}