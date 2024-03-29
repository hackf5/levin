include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    fetch_vk_bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
    GIT_TAG        v1.3.278
)
FetchContent_MakeAvailable(fetch_vk_bootstrap)

find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)

# MAIN PROJECT
file(
    GLOB_RECURSE
    LEVIN_SOURCES
    *.cpp)

add_executable(
    ${PROJECT_NAME}
    ${LEVIN_SOURCES})

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    OpenGL::GL
    Vulkan::Vulkan
    glm::glm
    vk-bootstrap::vk-bootstrap
    spdlog::spdlog
    glfw
    imgui
    stb
    ${CMAKE_DL_LIBS})