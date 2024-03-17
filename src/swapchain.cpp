#include "swapchain.h"

#include "spdlog/spdlog.h"

using namespace levin;

Swapchain::Swapchain(const Device &device_components):
    m_factory(SwapchainFactory(device_components)),
    m_swapchain(m_factory.create_swapchain()),
    m_swapchain_images(m_swapchain.get_images().value()),
    m_swapchain_image_views(m_factory.create_image_views(m_swapchain)),
    m_viewport(create_viewport()),
    m_scissor(create_scissor())
{
}

VkViewport Swapchain::create_viewport()
{
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_swapchain.extent.width);
    viewport.height = static_cast<float>(m_swapchain.extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    return viewport;
}

VkRect2D Swapchain::create_scissor()
{
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = m_swapchain.extent;

    return scissor;
}