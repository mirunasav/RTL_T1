cmake_minimum_required(VERSION 3.21)
project(Tema1)

set(CMAKE_CXX_STANDARD 14)

add_executable(client client.cpp server.h client.h)
add_executable(server server.cpp)


