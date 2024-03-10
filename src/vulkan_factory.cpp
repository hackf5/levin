#include "vulkan_factory.h"

#include <algorithm>
#include <stdexcept>

#include "spdlog/spdlog.h"

using namespace levin;

VulkanFactory::VulkanFactory(const vkb::Device &device)
    : m_device(device)
{
}

VulkanFactory::~VulkanFactory()
{
    std::reverse(m_destruction_queue.begin(), m_destruction_queue.end());
    for (auto &destruction : m_destruction_queue)
    {
        try
        {
            destruction(m_device);
        }
        catch (const std::exception &e)
        {
            spdlog::error("Failed to destroy resource: {}", e.what());
        }
    }
}