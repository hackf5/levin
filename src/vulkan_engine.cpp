#include "vulkan_engine.h"

#include "spdlog/spdlog.h"

using namespace levin;

VulkanEngine::VulkanEngine(
    const std::shared_ptr<WindowComponents> &window_components,
    const std::shared_ptr<DeviceComponents> &device_components):
    m_window_components(window_components),
    m_device_components(device_components),
    m_render_pass(RenderPassComponents(device_components)),
    m_swapchain(std::make_unique<SwapchainComponents>(device_components, m_render_pass.get_render_pass())),
    m_graphics_pipeline(GraphicsPipelineComponents(device_components, m_render_pass.get_render_pass()))
{
    spdlog::info("Vulkan Engine is starting");
}
