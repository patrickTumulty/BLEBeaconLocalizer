
cmake_minimum_required(VERSION 3.10)

project(bletracker_server)

file(GLOB SOURCES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    uWebSockets
    asio
)
    

