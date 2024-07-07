cmake_minimum_required(VERSION 3.10)

project(MylapsRC)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

add_subdirectory(client)
add_subdirectory(server)

