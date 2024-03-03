#include "vulkan_engine.h"

#include "spdlog/spdlog.h"

using namespace levin;

VulkanEngine::VulkanEngine(
    const std::shared_ptr<WindowComponents> &window_components,
    const std::shared_ptr<DeviceComponents> &device_components):
    m_window_components(window_components),
    m_device_components(device_components),
    m_swapchain(std::make_unique<SwapchainComponents>(device_components))
{
    spdlog::info("Vulkan Engine is starting");
}
