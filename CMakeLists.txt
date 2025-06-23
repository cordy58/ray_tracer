cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


# Only compile main.cpp
add_executable(raytrace main.cpp)

# If your headers are in subfolders, add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}, external)
