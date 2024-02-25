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
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

# MAIN PROJECT

set(SOURCES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    OpenGL::GL 
    OpenGL::GLU 
    GLUT::GLUT
    glm::glm
    ${CMAKE_DL_LIBS})

# TEST PROJECT
set(TEST_PROJECT_NAME ${PROJECT_NAME}_test)
set(
    TEST_SOURCES
    test/test.cpp
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCES})

target_link_libraries(
    ${TEST_PROJECT_NAME}
    glm::glm
    GTest::gtest_main
    ${CMAKE_DL_LIBS}
)

include(GoogleTest)
gtest_discover_tests(${TEST_PROJECT_NAME})