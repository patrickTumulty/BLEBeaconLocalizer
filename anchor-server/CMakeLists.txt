
cmake_minimum_required(VERSION 3.10)

project(bletracker_server)

file(GLOB SOURCES
   src/main.cpp
   src/web_socket.cpp
   src/anchor_server.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    uWebSockets
    asio
    shared-types
)


