cmake_minimum_required(VERSION 3.24)
project(procedural_tree)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendor/vgl)

include_directories(vendor/vgl/include)

add_executable(procedural_tree src/main.cpp src/layer.cpp src/layer.h src/mesh.cpp src/mesh.h src/tree.cpp src/tree.h)
target_link_libraries(procedural_tree vgl)
