#pragma once

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class RenderPassFactory : public VulkanFactory
    {
    public:
        RenderPassFactory(vkb::Device& device);

        VkRenderPass create_render_pass(const VkRenderPassCreateInfo &create_info);
    };
}