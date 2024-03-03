#include "device_components.h"

#include "spdlog/spdlog.h"

using namespace levin;

DeviceComponents::DeviceComponents(
    WindowComponents &window_components,
    bool enable_validation_layers)
    : DeviceComponents()
{
    spdlog::info("Initializing Vulkan Engine Components");

    init_device(window_components, enable_validation_layers);

    init_queues();
}

DeviceComponents::~DeviceComponents()
{
    spdlog::info("Destroying Vulkan Engine Components");

    vkb::destroy_device(device);
    vkb::destroy_surface(instance.instance, surface);
    vkb::destroy_instance(instance);
}

void DeviceComponents::init_device(WindowComponents &window_components, bool enable_validation_layers)
{
    spdlog::info("Initializing Device");

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

    window_components.create_window_surface(instance.instance, &surface);

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
}

void DeviceComponents::init_queues()
{
    spdlog::info("Initializing Queues");

    auto graphics_queue_ret = device.get_queue(vkb::QueueType::graphics);
    if (!graphics_queue_ret)
    {
        throw std::runtime_error("Failed to get graphics queue: " + graphics_queue_ret.error().message());
    }

    graphics_queue = graphics_queue_ret.value();

    auto present_queue_ret = device.get_queue(vkb::QueueType::present);
    if (!present_queue_ret)
    {
        throw std::runtime_error("Failed to get present queue: " + present_queue_ret.error().message());
    }

    present_queue = present_queue_ret.value();

    auto transfer_queue_ret = device.get_queue(vkb::QueueType::transfer);
    if (!transfer_queue_ret)
    {
        throw std::runtime_error("Failed to get transfer queue: " + transfer_queue_ret.error().message());
    }

    transfer_queue = transfer_queue_ret.value();
}