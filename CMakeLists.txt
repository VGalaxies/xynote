cmake_minimum_required(VERSION 3.22)

project(xynote LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# https://stackoverflow.com/questions/33633056/undefined-reference-to-vtable-for-mainwindow
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(
  PROJECT_SOURCES
  main.cpp
  widgets/xynote.cpp widgets/xynote.h
  widgets/sider.cpp widgets/sider.h
  resources/xynote.qrc
  forms/xynote.ui
  forms/sider.ui
)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCES})

find_package(Qt5 CONFIG COMPONENTS Widgets WebEngineWidgets REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Widgets Qt5::WebEngineWidgets)
