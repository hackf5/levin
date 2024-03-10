#include "command_factory.h"

#include "spdlog/spdlog.h"

using namespace levin;

CommandFactory::CommandFactory(const vkb::Device& device)
    : VulkanFactory(device)
{
}

VkCommandPool CommandFactory::create_command_pool(const VkCommandPoolCreateInfo& create_info)
{
    spdlog::info("Creating Command Pool");

    VkCommandPool command_pool;
    if (vkCreateCommandPool(device(), &create_info, nullptr, &command_pool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create command pool");
    }

    register_destruction([=, this]()
        {
            spdlog::info("Destroying Command Pool");
            vkDestroyCommandPool(device(), command_pool, nullptr);
        });

    return command_pool;
}

std::vector<VkCommandBuffer> CommandFactory::create_command_buffers(const VkCommandBufferAllocateInfo& allocate_info)
{
    spdlog::info("Creating Command Buffers");

    std::vector<VkCommandBuffer> command_buffers(allocate_info.commandBufferCount);
    if (vkAllocateCommandBuffers(device(), &allocate_info, command_buffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers");
    }

    register_destruction([=, this]()
        {
            spdlog::info("Destroying Command Buffers");
            vkFreeCommandBuffers(device(), allocate_info.commandPool, command_buffers.size(), command_buffers.data());
        });

    return command_buffers;
}

std::vector<VkSemaphore> CommandFactory::create_semaphores(uint32_t count)
{
    spdlog::info("Creating Semaphores");

    std::vector<VkSemaphore> semaphores(count);
    for (auto& semaphore : semaphores)
    {
        VkSemaphoreCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(device(), &create_info, nullptr, &semaphore) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create semaphore");
        }
    }

    register_destruction([=, this]()
        {
            spdlog::info("Destroying Semaphores");
            for (auto& semaphore : semaphores)
            {
                vkDestroySemaphore(device(), semaphore, nullptr);
            }
        });

    return semaphores;
}

std::vector<VkFence> CommandFactory::create_fences(uint32_t count, VkFenceCreateFlags flags)
{
    spdlog::info("Creating Fences");

    std::vector<VkFence> fences(count);
    for (auto& fence : fences)
    {
        VkFenceCreateInfo create_info {};
        create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        create_info.flags = flags;

        if (vkCreateFence(device(), &create_info, nullptr, &fence) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create fence");
        }
    }

    register_destruction([=, this]()
        {
            spdlog::info("Destroying Fences");
            for (auto& fence : fences)
            {
                vkDestroyFence(device(), fence, nullptr);
            }
        });

    return fences;
}