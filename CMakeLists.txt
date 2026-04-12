
cmake_minimum_required(VERSION 3.28)

project(bletracker VERSION 0.1.0 LANGUAGES CXX)

# For C++ projects
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_subdirectory(submodules)

add_subdirectory(libs/shared_types)

add_subdirectory(server)

