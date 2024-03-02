#pragma once

#include "vulkan_engine_components.h"

namespace levin
{
    class VulkanEngine
    {
    private:
        VulkanEngineComponents *const m_components;

    public:
        VulkanEngine(VulkanEngineComponents &components);
    };
}