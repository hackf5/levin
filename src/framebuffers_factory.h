#pragma once

#include <vector>

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class FramebuffersFactory: public VulkanFactory
    {
    public:
        FramebuffersFactory(const vkb::Device &device);
        FramebuffersFactory(const FramebuffersFactory &) = delete;

        VkFramebuffer create_framebuffer(const VkFramebufferCreateInfo &create_info);
    };
}