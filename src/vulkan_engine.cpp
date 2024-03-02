#include "vulkan_engine.h"

#include <iostream>

using namespace levin;

VulkanEngine::VulkanEngine(VulkanEngineComponents &components)
    : m_components(&components)
{
    std::cout << "Starting Vulkan Engine" << std::endl;
}
