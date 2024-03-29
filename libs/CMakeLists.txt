add_library(stb STATIC ${CMAKE_CURRENT_LIST_DIR}/stb/lib.cpp)
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_LIST_DIR}/stb)

add_library(tiny_object_loader STATIC ${CMAKE_CURRENT_LIST_DIR}/tinyobjloader/lib.cpp)
target_include_directories(tiny_object_loader PUBLIC ${CMAKE_CURRENT_LIST_DIR}/tinyobjloader)