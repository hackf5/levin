#include "vulkan_factory.h"

#include <algorithm>

#include "spdlog/spdlog.h"

using namespace levin;

VulkanFactory::VulkanFactory(vkb::Device &device)
    : m_device(&device)
{
}

VulkanFactory::~VulkanFactory()
{
    spdlog::info("Destroying Vulkan Factory");

    std::reverse(m_destruction_queue.begin(), m_destruction_queue.end());
    for (auto &destroy : m_destruction_queue)
    {
        destroy();
    }
}

void VulkanFactory::register_destruction(destruction_callback_t destroy)
{
    m_destruction_queue.push_back(destroy);
}

vkb::Device &VulkanFactory::get_device()
{
    return *m_device;
}