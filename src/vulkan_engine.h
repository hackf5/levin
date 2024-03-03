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
        std::unique_ptr<SwapchainComponents> m_swapchain;

        WindowComponents *const m_window_components;
        DeviceComponents *const m_device_components;

    public:
        VulkanEngine(
            WindowComponents &window_components,
            DeviceComponents &device_components);
    };
}