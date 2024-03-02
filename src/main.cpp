#include <iostream>

#include "glfw_window.h"
#include "vulkan_engine_components.h"
#include "vulkan_engine.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

int main()
{
    std::cout << "Starting Levin" << std::endl;

    try
    {
        levin::GLFWWindow window(800, 600, "Levin Engine");
        levin::VulkanEngineComponents engineComponents(window, enableValidationLayers);
    }
    catch (const std::exception &e)
    {
        std::cout << "Fatal: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Stopping Levin" << std::endl;

    return EXIT_SUCCESS;
}