cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)

set(PROJECT_NAME levin)
project(${PROJECT_NAME} LANGUAGES CXX)

set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin/$<LOWER_CASE:$<CONFIG>>)

add_subdirectory(submodules)
add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(shaders)
add_subdirectory(assets)