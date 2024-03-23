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

# MAIN PROJECT
file(
    GLOB_RECURSE
    SOURCES
    *.cpp)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/inc/imgui)

file(
    GLOB
    IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/*.h
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.*
    ${IMGUI_DIR}/backends/imgui_impl_glfw.*
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.*)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
    ${IMGUI_SOURCES})

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    .
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/inc/imgui)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    OpenGL::GL
    Vulkan::Vulkan
    glm::glm
    glfw
    vk-bootstrap::vk-bootstrap
    ${CMAKE_DL_LIBS})