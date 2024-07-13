cmake_minimum_required(VERSION 3.10)
project(MylapsRC)
set(USE_BOOST "Use Boost.Asio for networking" TRUE)
set(SOURCES
    src/main.cpp
    src/server.cpp
    src/server_manager.cpp
    src/server_socket.cpp
    src/message_analyser.cpp
    src/race_analyser.cpp
    src/driver.cpp
    include/server.h
    include/server_manager.h
    include/server_socket.h
    include/message_analyser.h
    include/race_analyser.h
    include/driver.h
)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")

#set(BOOST_ROOT "C:/boost")
#set(BOOST_INCLUDEDIR "C:/boost")
#include_directories(${BOOST_INCLUDEDIR})
#set(BOOST_LIBRARYDIR "C:/boost/lib")
#link_directories(${BOOST_LIBRARYDIR})

if(USE_BOOST)
    find_package(Boost REQUIRED COMPONENTS system)
    include_directories(${Boost_INCLUDE_DIRS})
    add_definitions(-DUSE_BOOST)
endif()
if (WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
    add_compile_definitions(NOMINMAX)
endif()

add_executable(server_app ${SOURCES})

if (WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
    add_compile_definitions(NOMINMAX)
    if(USE_BOOST)
        target_link_libraries(server_app Boost::system ws2_32 mswsock)
    else()
        target_link_libraries(server_app ws2_32 mswsock advapi32)
    endif()
elseif (UNIX)
    if(USE_BOOST)
        target_link_libraries(server_app Boost::system pthread)
    else()
        target_link_libraries(server_app pthread)
    endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(tests)


