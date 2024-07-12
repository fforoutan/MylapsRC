cmake_minimum_required(VERSION 3.10)
project(MylapsRC)


include_directories(${PROJECT_SOURCE_DIR}../include)

file(GLOB SERVER_SOURCES
    ${PROJECT_SOURCE_DIR}/../src/driver.cpp
    ${PROJECT_SOURCE_DIR}/../src/server.cpp
    ${PROJECT_SOURCE_DIR}/../src/message_handler.cpp
    ${PROJECT_SOURCE_DIR}/../src/race_analyser.cpp
    ${PROJECT_SOURCE_DIR}/../src/server_socket.cpp
    ${PROJECT_SOURCE_DIR}/../include/server_socket.h
#    ${PROJECT_SOURCE_DIR}/test_driver.cpp
#    ${PROJECT_SOURCE_DIR}/test_server.cpp
#    ${PROJECT_SOURCE_DIR}/test_message_handler.cpp
#    ${PROJECT_SOURCE_DIR}/test_race_analyser.cpp
     ${PROJECT_SOURCE_DIR}/test_server_socket.cpp
)

add_executable(test_server ${SERVER_SOURCES} test_server_socket.cpp)

add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")

######################################## Google Test and Mock ################################
include(FetchContent)
FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/release-1.11.0.zip)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)
target_link_libraries(test_server gtest gmock gtest_main)
target_link_libraries(test_server ws2_32 mswsock advapi32)
###########################################################################################

enable_testing()
