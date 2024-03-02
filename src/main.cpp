#include <iostream>

#include "glfw_window.h"
#include "vulkan_engine.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

int main() {
    levin::GLFWWindow window(800, 600, "Levin Engine");
    levin::VulkanEngine engine(window, enableValidationLayers);

    try {
        engine.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}