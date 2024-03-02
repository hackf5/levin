#include "vulkan_engine.h"

#include <iostream>
#include <vector>

#include "VkBootstrap.h"

using namespace levin;

VulkanEngine::VulkanEngine(
    GLFWWindow &window,
    bool enable_validation_layers)
    : m_window(&window), m_enable_validation_layers(enable_validation_layers)
{
}

VulkanEngine::~VulkanEngine()
{
    std::cout << "Destroying Vulkan Engine" << std::endl;

    vkb::destroy_device(m_device);
    vkb::destroy_surface(m_instance.instance, m_surface, nullptr);
    vkb::destroy_instance(m_instance);
}

void VulkanEngine::run()
{
    std::cout << "Starting Vulkan Engine" << std::endl;

    init_vulkan();
}

void VulkanEngine::init_vulkan()
{
    std::cout << "Initializing Vulkan" << std::endl;

    vkb::InstanceBuilder builder;
    auto inst_ret = builder.set_app_name("Example Vulkan Application")
        .request_validation_layers(m_enable_validation_layers)
        .use_default_debug_messenger()
        .build();
    if (!inst_ret)
    {
        throw std::runtime_error("Failed to create Vulkan Instance: " + inst_ret.error().message());
    }
    m_instance = inst_ret.value();

    m_window->create_window_surface(m_instance.instance, &m_surface);

    vkb::PhysicalDeviceSelector selector { m_instance };
    auto phys_ret = selector.set_surface(m_surface)
        .set_minimum_version(1, 3)
        .require_dedicated_transfer_queue()
        .select();
    if (!phys_ret)
    {
        throw std::runtime_error("Failed to select Vulkan Physical Device: " + phys_ret.error().message());
    }

    vkb::DeviceBuilder device_builder { phys_ret.value() };
    auto dev_ret = device_builder.build();
    if (!dev_ret)
    {
        throw std::runtime_error("Failed to create Vulkan Device: " + dev_ret.error().message());
    }
    m_device = dev_ret.value();

    // Get the graphics queue with a helper function
    auto graphics_queue_ret = m_device.get_queue(vkb::QueueType::graphics);
    if (!graphics_queue_ret)
    {
        throw std::runtime_error("Failed to get Vulkan Graphics Queue: " + graphics_queue_ret.error().message());
    }
    m_graphics_queue = graphics_queue_ret.value();
}
