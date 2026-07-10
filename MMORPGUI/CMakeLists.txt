cmake_minimum_required(VERSION 3.16)

project(MMORPGUI LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(UI)
