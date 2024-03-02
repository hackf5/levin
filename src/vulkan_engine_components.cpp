#include "vulkan_engine_components.h"

#include <iostream>

using namespace levin;

VulkanEngineComponents::VulkanEngineComponents(
    GLFWWindow &window,
    bool enable_validation_layers)
    : VulkanEngineComponents()
{
    std::cout << "Creating Vulkan Engine Components" << std::endl;

    init_device(window, enable_validation_layers);

    throw std::runtime_error("Failed to create Vulkan Engine Components");

    init_swapchain();
}

VulkanEngineComponents::~VulkanEngineComponents()
{
    std::cout << "Destroying Vulkan Engine Components" << std::endl;

    vkb::destroy_swapchain(swapchain);
    vkb::destroy_device(device);
    vkb::destroy_surface(instance.instance, surface);
    vkb::destroy_instance(instance);
}

void VulkanEngineComponents::init_device(GLFWWindow &window, bool enable_validation_layers)
{
    std::cout << "Initializing Vulkan Device" << std::endl;

    vkb::InstanceBuilder builder;
    auto inst_ret = builder.set_app_name("Levin")
        .request_validation_layers(enable_validation_layers)
        .use_default_debug_messenger()
        .build();
    if (!inst_ret)
    {
        throw std::runtime_error("Failed to create Vulkan Instance: " + inst_ret.error().message());
    }
    instance = inst_ret.value();

    window.create_window_surface(instance.instance, &surface);

    vkb::PhysicalDeviceSelector selector { instance };
    auto phys_ret = selector.set_surface(surface)
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
    device = dev_ret.value();

    auto graphics_queue_ret = device.get_queue(vkb::QueueType::graphics);
    if (!graphics_queue_ret)
    {
        throw std::runtime_error("Failed to get Vulkan Graphics Queue: " + graphics_queue_ret.error().message());
    }
    graphics_queue = graphics_queue_ret.value();
}

void VulkanEngineComponents::init_swapchain()
{
    std::cout << "Initializing Vulkan Swapchain" << std::endl;

    vkb::SwapchainBuilder swapchain_builder { device };
    auto swapchain_ret = swapchain_builder
        .set_old_swapchain(swapchain)
        .build();

    if (!swapchain_ret)
    {
        throw std::runtime_error("Failed to create Vulkan Swapchain: " + swapchain_ret.error().message());
    }

    vkb::destroy_swapchain(swapchain);
    swapchain = swapchain_ret.value();
}
