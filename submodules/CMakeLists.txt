
cmake_minimum_required(VERSION 3.10)

project(submodules)

add_subdirectory(zlib)

################################################################################
# ASIO
################################################################################
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE asio/asio/include)
################################################################################

################################################################################
# uWebSockets
################################################################################
find_package(Threads REQUIRED)

set(UWS_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/uWebSockets)
set(USOCK_DIR ${UWS_DIR}/uSockets/src)

file(GLOB USOCKETS_SOURCES
    ${USOCK_DIR}/*.c
    ${USOCK_DIR}/eventing/*.c
)

add_library(uSockets STATIC ${USOCKETS_SOURCES})

target_include_directories(uSockets PUBLIC
    ${USOCK_DIR}
)

target_compile_definitions(uSockets PUBLIC
    LIBUS_NO_SSL
)

target_link_libraries(uSockets PUBLIC
    ZLIB::ZLIB
    Threads::Threads
)

add_library(uWebSockets INTERFACE)

target_include_directories(uWebSockets INTERFACE ${UWS_DIR}/src)

target_link_libraries(uWebSockets INTERFACE uSockets)
################################################################################
