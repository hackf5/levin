#define GLM_FORCE_RADIANS

#include <memory>

#include "window_components.h"
#include "device_components.h"
#include "vulkan_engine.h"

#include "spdlog/spdlog.h"

using namespace levin;

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
        auto window = std::make_shared<WindowComponents>(800, 600, "Levin");
        auto device = std::make_shared<DeviceComponents>(window, enableValidationLayers);
        VulkanEngine engine(window, device);

        engine.run();
    }
    catch (const std::exception &e)
    {
        spdlog::error("Fatal: {}", e.what());
        return EXIT_FAILURE;
    }

    spdlog::info("Stopping Levin");

    return EXIT_SUCCESS;
}