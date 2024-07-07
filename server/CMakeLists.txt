cmake_minimum_required(VERSION 3.10)
project(MylapsRC)
set(SOURCES
    src/main.cpp
    src/server.cpp
    src/server_manager.cpp
    src/server_socket.cpp
    include/server.h
    include/server_manager.h
    include/server_socket.h
)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")
if (WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
    add_compile_definitions(NOMINMAX)
endif()

add_executable(server_app ${SOURCES})

if (MINGW)
    target_link_libraries(server_app ws2_32 mswsock advapi32)
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


