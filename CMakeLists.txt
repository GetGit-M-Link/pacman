cmake_minimum_required(VERSION 3.19)
project(klausur)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
include_directories(.)
find_package(Qt5Widgets REQUIRED)
add_executable(klausur
        pacman.cpp
        pacman.h
        ConsoleWindow.cpp
        ConsoleWindow.h
        main.cpp)
target_link_libraries(klausur Qt5::Widgets)
