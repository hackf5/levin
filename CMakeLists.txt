cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)

set(PROJECT_NAME levin)
project(${PROJECT_NAME} LANGUAGES CXX)

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

set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "bin")

# MAIN PROJECT
file(
    GLOB_RECURSE
    SOURCES
    "src/*.cpp")

file(
    GLOB
    IMGUI_SOURCES
    "inc/imgui/*.cpp"
    "inc/imgui/*.h"
    "inc/imgui/misc/cpp/imgui_stdlib.*"
    "inc/imgui/backends/imgui_impl_glfw.*"
    "inc/imgui/backends/imgui_impl_vulkan.*")

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
    ${IMGUI_SOURCES})

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    src
    src/models
    src/gui
    inc
    inc/imgui
    inc/imgui/backends)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    OpenGL::GL
    Vulkan::Vulkan
    glm::glm
    glfw
    vk-bootstrap::vk-bootstrap
    ${CMAKE_DL_LIBS})

# Shaders
if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
  set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
else()
  set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "shaders/*.frag"
    "shaders/*.vert"
    )

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  string(REPLACE "shader." "" FILE_NAME ${FILE_NAME})
  set(SPIRV "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/"
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
    )

add_dependencies(${PROJECT_NAME} Shaders)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)