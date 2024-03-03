#include "vulkan_engine.h"

#include "spdlog/spdlog.h"

using namespace levin;

VulkanEngine::VulkanEngine(
    WindowComponents &window_components,
    DeviceComponents &device_components):
    m_window_components(&window_components),
    m_device_components(&device_components)
{
    spdlog::info("Vulkan Engine is starting");

    m_swapchain = std::make_unique<SwapchainComponents>(device_components);
}
