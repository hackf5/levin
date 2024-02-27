cmake_minimum_required(VERSION 3.27)

set(PROJECT_NAME levin)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# Set build type to debug by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	33b0eb9fa336ffd8551024b1d2690e418014553b #refs/tags/0.9.9.8
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GTest REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "bin")

# MAIN PROJECT
set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    OpenGL::GL
    Vulkan::Vulkan
    glm::glm
    glfw
    ${CMAKE_DL_LIBS})

# TEST PROJECT
set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)
file(GLOB TEST_SOURCES
    "test/*.cpp"
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCES})

target_include_directories(${TEST_PROJECT_NAME} PRIVATE src)

target_link_libraries(
    ${TEST_PROJECT_NAME}
    glm::glm
    GTest::gtest_main
    ${CMAKE_DL_LIBS}
)

include(GoogleTest)
gtest_discover_tests(${TEST_PROJECT_NAME})

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