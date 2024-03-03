#pragma once

#include <memory>

#include "window_components.h"
#include "device_components.h"
#include "swapchain_components.h"

namespace levin
{
    class VulkanEngine
    {
    private:
        std::shared_ptr<WindowComponents> m_window_components;
        std::shared_ptr<DeviceComponents> m_device_components;

        std::unique_ptr<SwapchainComponents> m_swapchain;

    public:
        VulkanEngine(
            const std::shared_ptr<WindowComponents>& window_components,
            const std::shared_ptr<DeviceComponents> &device_components);
    };
}