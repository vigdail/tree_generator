cmake_minimum_required(VERSION 3.24)
project(procedural_tree)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        rttr
        GIT_REPOSITORY https://github.com/rttrorg/rttr.git
        GIT_TAG master
)
FetchContent_MakeAvailable(rttr)

add_subdirectory(vendor/vgl)

include_directories(vendor/vgl/include)

add_executable(procedural_tree src/main.cpp src/layer.cpp src/layer.h src/mesh.cpp src/mesh.h src/tree.cpp src/tree.h src/bound_box.cpp src/bound_box.h src/node.cpp src/ligth.cpp src/light.h src/reflection.h)
target_link_libraries(procedural_tree vgl RTTR::Core)
