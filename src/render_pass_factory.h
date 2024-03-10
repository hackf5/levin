#pragma once

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class RenderPassFactory : public VulkanFactory
    {
    public:
        RenderPassFactory(const vkb::Device& device);
        RenderPassFactory(const RenderPassFactory&) = delete;

        VkRenderPass create_render_pass(const VkRenderPassCreateInfo &create_info);
    };
}