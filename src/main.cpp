#define GLM_FORCE_RADIANS

#include <memory>

#include "spdlog/spdlog.h"

#include "vulkan_context_builder.h"
#include "vulkan_engine.h"

using namespace levin;

int main()
{
    spdlog::info("Starting Levin");

#ifdef NDEBUG
    const bool enableValidationLayers = false;
    spdlog::set_level(spdlog::level::trace);
#else
    const bool enableValidationLayers = true;
    spdlog::set_level(spdlog::level::debug);
#endif
    try
    {
        auto layout_builder = [](DescriptorSetLayoutBuilder &builder)
        {
            builder
                .add_uniform_buffer() // camera
                .add_uniform_buffer() // model
                .add_combined_image_sampler(); // texture
        };

        auto context = VulkanContextBuilder()
            .add_window(800, 600, "Levin")
            .add_device(enableValidationLayers)
            .add_graphics_queue()
            .add_adhoc_queues()
            .add_sampler()
            .add_graphics_buffers()
            .add_descriptor_set_layout(layout_builder)
            .add_scene()
            .add_swapchain()
            .add_depth_buffer()
            .add_render_pass()
            .add_framebuffers()
            .add_graphics_pipeline()
            .add_gui()
            .build();

        VulkanEngine(std::move(context)).run();
    }
    catch (const std::exception &e)
    {
        spdlog::error("Fatal: {}", e.what());
        return EXIT_FAILURE;
    }

    spdlog::info("Stopping Levin");

    return EXIT_SUCCESS;
}