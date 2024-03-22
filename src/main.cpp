#define GLM_FORCE_RADIANS

#include <memory>

#include "spdlog/spdlog.h"

#include "vulkan_context_builder.h"
#include "vulkan_engine.h"


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
        auto context = VulkanContextBuilder()
            .add_window(800, 600, "Levin")
            .add_device(enableValidationLayers)
            .add_graphics_queue()
            .add_transfer_queue()
            .add_descriptor_pool()
            .add_descriptor_set_layout()
            .add_model()
            .add_camera()
            .add_swapchain()
            .add_render_pass()
            .add_framebuffers()
            .add_graphics_pipeline()
            .add_gui()
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