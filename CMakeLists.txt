cmake_minimum_required(VERSION 3.22)
project(xynote LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
# https://stackoverflow.com/questions/33633056/undefined-reference-to-vtable-for-mainwindow
set(CMAKE_AUTOMOC ON)

add_executable(main)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.h)
target_sources(main PUBLIC ${sources})

# message(STATUS "Sources: ${sources}")

find_package(Qt5 CONFIG COMPONENTS Widgets Gui WebEngineWidgets WebChannel REQUIRED)
target_link_libraries(main PUBLIC Qt5::Widgets Qt5::Gui Qt5::WebEngineWidgets Qt5::WebChannel)
