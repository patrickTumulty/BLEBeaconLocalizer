
cmake_minimum_required(VERSION 3.10)

project(bletracker_server)

file(GLOB SOURCES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC websocketpp_lib asio)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    _WEBSOCKETPP_CPP11_STL_
    ASIO_STANDALONE
)

