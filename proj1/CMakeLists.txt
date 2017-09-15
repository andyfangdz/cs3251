cmake_minimum_required(VERSION 3.8)
project(proj1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(CLIENT_SOURCE_FILES
        client.cpp)

set(SERVER_SOURCE_FILES
        server.cpp)

add_executable(client ${CLIENT_SOURCE_FILES})
add_executable(server ${SERVER_SOURCE_FILES})
