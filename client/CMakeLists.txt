set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Network Qml Quick REQUIRED)

add_library(client_lib src/client.cpp resources/client.qrc)

include_directories(${PROJECT_SOURCE_DIR}/include)
target_include_directories(client_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Qml_INCLUDE_DIRS}
    ${Qt5Quick_INCLUDE_DIRS}
)


target_link_libraries(client_lib Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Qml Qt5::Quick)

add_executable(client_app src/main.cpp src/client.cpp include/client.h resources/client.qrc)
target_link_libraries(client_app client_lib)
