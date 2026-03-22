
cmake_minimum_required(VERSION 3.10)

project(submodules)

################################################################################
# ASIO
################################################################################
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE asio/asio/include)
################################################################################

################################################################################
# Websocket++
################################################################################
add_library(websocketpp_lib INTERFACE)
target_include_directories(websocketpp_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/websocketpp
)
target_compile_definitions(websocketpp_lib INTERFACE
    ASIO_STANDALONE
    _WEBSOCKETPP_CPP11_STL_
)
target_link_libraries(websocketpp_lib INTERFACE asio)
################################################################################
