cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


add_executable(raytrace main.cpp)
add_executable(raytrace_old main-deprecated.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}, external)
