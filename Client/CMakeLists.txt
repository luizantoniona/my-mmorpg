cmake_minimum_required(VERSION 3.16)

project(
  MMORPGClient
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MMORPGCLIENT_BUILD_TESTS "Build tests" OFF)

if(DEFINED ENV{VCPKG_ROOT})
  file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT_CMAKE)
  set(CMAKE_TOOLCHAIN_FILE
      "${VCPKG_ROOT_CMAKE}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Qml)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(App)

if(MMORPGCLIENT_BUILD_TESTS)
  enable_testing()
  add_subdirectory(Test)
endif()
