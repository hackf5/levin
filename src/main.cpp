#define GLM_FORCE_RADIANS

#include <memory>

#include "vulkan_context_builder.h"
#include "vertex.h"
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
        VulkanContextBuilder builder;
        auto context = builder.configure_window(800, 600, "Levin")
            .configure_device(enableValidationLayers)
            .configure_graphics_queue()
            .configure_transfer_queue()
            .configure_descriptor_pool()
            .configure_descriptor_set_layout()
            .configure_model()
            .configure_camera()
            .configure_swapchain()
            .configure_render_pass()
            .configure_framebuffers()
            .configure_graphics_pipeline()
            .build();

        VulkanEngine engine(std::move(context));

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