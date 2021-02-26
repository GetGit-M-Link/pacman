cmake_minimum_required(VERSION 3.17)
project(klausur)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
include_directories(.)
find_package(Qt5Widgets REQUIRED)
add_executable(klausur
        pacman.cpp
        pacman.h
        characters.cpp
        characters.h
        ConsoleWindow.cpp
        ConsoleWindow.h
        main.cpp)
target_link_libraries(klausur Qt5::Widgets)
