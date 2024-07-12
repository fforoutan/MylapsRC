cmake_minimum_required(VERSION 3.10)
project(MylapsRC)


include_directories(${PROJECT_SOURCE_DIR}../include)

file(GLOB SERVER_SOURCES
    ${PROJECT_SOURCE_DIR}/../src/driver.cpp
    ${PROJECT_SOURCE_DIR}/../src/server.cpp
    ${PROJECT_SOURCE_DIR}/../src/message_handler.cpp
    ${PROJECT_SOURCE_DIR}/../src/race_analyser.cpp
#    ${PROJECT_SOURCE_DIR}/test_driver.cpp
#    ${PROJECT_SOURCE_DIR}/test_server.cpp
#    ${PROJECT_SOURCE_DIR}/test_message_handler.cpp
    ${PROJECT_SOURCE_DIR}/test_race_analyser.cpp
)

add_executable(test_app ${SERVER_SOURCES})

add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")


######################################## BOOST ############################################
#set(BOOST_ROOT "C:/boost")
#set(BOOST_INCLUDEDIR "C:/boost")
#include_directories(${BOOST_INCLUDEDIR})
#set(BOOST_LIBRARYDIR "C:/boost/lib")
#link_directories(${BOOST_LIBRARYDIR})

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
target_link_libraries(test_app Boost::unit_test_framework)
target_link_libraries(test_app
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )

###########################################################################################

enable_testing()
