#pragma once

#include <vector>

#include "VkBootstrap.h"

#include "vulkan_factory.h"

namespace levin
{
    class CommandFactory : public VulkanFactory
    {
    public:
        CommandFactory(vkb::Device& device);

        VkCommandPool create_command_pool(const VkCommandPoolCreateInfo& create_info);

        std::vector<VkCommandBuffer> create_command_buffers(const VkCommandBufferAllocateInfo& allocate_info);

        std::vector<VkSemaphore> create_semaphores(uint32_t count);

        std::vector<VkFence> create_fences(uint32_t count, VkFenceCreateFlags flags = VK_FENCE_CREATE_SIGNALED_BIT);
    };
}