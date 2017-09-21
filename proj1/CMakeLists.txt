cmake_minimum_required(VERSION 3.8)
project(proj1)

set(LIBRARIES "crypto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(CLIENT_SOURCE_FILES
        client.cpp mini-thrift.h mini-thrift.cpp)

set(SERVER_SOURCE_FILES
        server.cpp mini-thrift.h mini-thrift.cpp)

add_executable(client ${CLIENT_SOURCE_FILES})
target_link_libraries(client ${LIBRARIES})

add_executable(server ${SERVER_SOURCE_FILES})
target_link_libraries(server ${LIBRARIES})
