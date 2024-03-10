#define GLM_FORCE_RADIANS

#include <memory>

#include "vulkan_context.h"
#include "vertex.h"
#include "uniform_buffer_object.h"
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
            .configure_transfer_queue()
            .configure_descriptor_pool()
            .configure_render_pass()
            .configure_swapchain()
            .configure_graphics_pipeline()
            .configure_graphics_commands()
            .configure_vertex_buffer(sizeof(Vertex) * 100)
            .configure_index_buffer(sizeof(Vertex::index_t) * 100)
            .configure_uniform_buffers(sizeof(UniformBufferObject))
            .configure_uniform_buffer_descriptor_set(sizeof(UniformBufferObject))
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