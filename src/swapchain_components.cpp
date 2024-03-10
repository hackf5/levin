#include "swapchain_components.h"

#include "spdlog/spdlog.h"

using namespace levin;

SwapchainComponents::SwapchainComponents(const DeviceComponents &device_components):
    m_factory(SwapchainFactory(device_components)),
    m_swapchain(m_factory.create_swapchain()),
    m_swapchain_images(m_swapchain.get_images().value()),
    m_swapchain_image_views(m_factory.create_image_views(m_swapchain)),
    m_viewport(VkViewport{}),
    m_scissor(VkRect2D{})
{
    m_viewport.x = 0.0f;
    m_viewport.y = 0.0f;
    m_viewport.width = static_cast<float>(m_swapchain.extent.width);
    m_viewport.height = static_cast<float>(m_swapchain.extent.height);
    m_viewport.minDepth = 0.0f;
    m_viewport.maxDepth = 1.0f;

    m_scissor.offset = {0, 0};
    m_scissor.extent = m_swapchain.extent;
}