#include "window_components.h"
#include "device_components.h"
#include "vulkan_engine.h"

#include "spdlog/spdlog.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

int main()
{
    spdlog::info("Starting Levin");

#ifdef NDEBUG
    spdlog::set_level(spdlog::level::trace);
#else
    spdlog::set_level(spdlog::level::debug);
#endif

    try
    {
        levin::WindowComponents windowComponents(800, 600, "Levin Engine");
        levin::DeviceComponents deviceComponents(windowComponents, enableValidationLayers);
        levin::VulkanEngine engine(windowComponents, deviceComponents);
    }
    catch (const std::exception &e)
    {
        spdlog::error("Fatal: {}", e.what());
        return EXIT_FAILURE;
    }

    spdlog::info("Stopping Levin");

    return EXIT_SUCCESS;
}