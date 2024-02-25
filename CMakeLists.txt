cmake_minimum_required(VERSION 3.27)

set(PROJECT_NAME levin)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# Set build type to debug by default.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

# Add your source files here
set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL OpenGL::GLU GLUT::GLUT ${CMAKE_DL_LIBS})

# TESTS
set(
    TEST_PROJECT_NAME
    ${PROJECT_NAME}_test
)
set(
    TEST_SOURCES
    test/test.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(${TEST_PROJECT_NAME} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${TEST_PROJECT_NAME})