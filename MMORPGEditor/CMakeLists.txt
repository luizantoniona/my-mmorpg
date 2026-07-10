cmake_minimum_required(VERSION 3.16)

project(MMORPGEditor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MMORPGEDITOR_BUILD_TESTS "Build tests" OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(Editor)

if(MMORPGEDITOR_BUILD_TESTS)
  enable_testing()
  add_subdirectory(Test)
endif()
