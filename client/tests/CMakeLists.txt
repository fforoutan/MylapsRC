cmake_minimum_required(VERSION 3.10)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

project(MylapsRC)


include_directories(${PROJECT_SOURCE_DIR}/../include)

file(GLOB CLIENT_SOURCES
    ${PROJECT_SOURCE_DIR}/../src/client.cpp
    ${PROJECT_SOURCE_DIR}/../include/client.h
    ${PROJECT_SOURCE_DIR}/test_client.cpp
#    ${PROJECT_SOURCE_DIR}/mock_qtcpsocket.h
#    ${PROJECT_SOURCE_DIR}/testtable_client.h
)

add_executable(test_client ${CLIENT_SOURCES}
    test_client.cpp)

add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")

target_link_libraries(test_client client_lib)
######################################## BOOST ############################################

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
target_link_libraries(test_client Boost::unit_test_framework)
target_link_libraries(test_client
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                      )
######################################## QT ###############################################
find_package(Qt5 COMPONENTS Core Gui Widgets Network Qml Quick Test REQUIRED)
target_include_directories(test_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
    ${Qt5Test_INCLUDE_DIRS}
)
target_link_libraries(test_client Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Qml Qt5::Quick Qt5::Test)
#####################################################################################################
enable_testing()
