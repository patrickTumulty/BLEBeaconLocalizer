
set(SOURCES
    message_types.c
)

if(ESP_PLATFORM)
    message(STATUS "Building shared-types for ESP-IDF")
    idf_component_register(
        SRCS ${SOURCES}
        INCLUDE_DIRS "."
    )
else()
    message(STATUS "Building shared-types for standard CMake")

    cmake_minimum_required(VERSION 3.10)

    project(shared-types)

    add_library(${PROJECT_NAME} STATIC ${SOURCES})

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

